#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
#include <QWheelEvent>
#include <QMouseEvent>
#include "vefmodel.hpp"
#include "openglwindow.hpp"
#include "quality.hpp"
#include "octree.hpp"
#include "Ray.hpp"
#include <ctime>
#include <iomanip>
#include <QGLFormat>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

typedef std::vector<float> ASC;

struct Camera
{
    QVector3D position;
    QVector3D direction;
    QVector3D right;
    QVector3D up;
};

class ViewWindow : public OpenGLWindow
{
public:
    ViewWindow() : m_VertexBuffer(QOpenGLBuffer::VertexBuffer)
    {
        loadASC("sphere.asc", m_points);
        //loadASC("Grp1-2014_Simplified.asc", m_points);

        //std::vector<int> indexes;
        indexes.resize(m_points.size()/6);
        for(unsigned int i = 0; i < indexes.size(); ++i)
            indexes[i] = i;

        Octree octree(m_points, indexes, 10, 200, 100.f, QVector3D(mx, my, mz), 0);
        std::cout << "nbLeaf : " << octree.getNbLeaf() << std::endl;
        octree.decimation(m_DecIndexes, m_DecVertices, 0);

        m_camera.position.setZ(-10.f);

        //m_DecIndexes.resize(100);
        //m_DecVertices.reserve(100);

        //std::vector<float> res = octree.getNbOf(QVector3D(m_points[6*57+0], m_points[6*57+1], m_points[6*57+2]), 10.f);

        //for(unsigned int i = 0; i < res.size(); i += 6)
        //    std::cout << "(" << res[i+0] << " ; " << res[i+1] << " ; " << res[i+2] << ")" << std::endl;
    }
    ~ViewWindow(){delete m_Data;}
    void initialize();
    void render();
    void updatePoints(bool drawDecVertices);

private:
    GLuint loadShader(GLenum type, const char *source)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        return shader;
    }

    void loadASC(const std::string& filename, ASC& result);

    void mouseMoveEvent(QMouseEvent *eventMove);
    void mousePressEvent(QMouseEvent *eventPress);
    void mouseReleaseEvent(QMouseEvent *releaseEvent);
    void wheelEvent(QWheelEvent *event);

    static const std::string vertexShaderSource;
    static const std::string fragmentShaderSource;

    VEFModel mesh;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    GLuint m_zoomUniform;

    QMatrix4x4 m_MVPMatrix;


    struct float6{
        float x,y,z;
        float r,g,b;
    };

    float6* m_Data;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    QOpenGLVertexArrayObject m_VAO;
    GLuint m_VertexBuffer;
    GLuint vbo;

    QPoint m_LastPos;
    QVector3D m_lookAt;
    bool m_Rotate;
    bool m_UpdateRender;
    float m_AngleX, m_AngleY;
    float m_RotateSpeed;
    float m_ZoomFactor;

    ASC m_points;
    std::vector<int> indexes;
    unsigned int pointsCount;
    GLfloat* vertices;
    GLfloat* colors;
    float m_scale;
    float mx, my, mz;

    Camera m_camera;
    Octree* octree;
    Ray m_ray;
    std::vector<int> m_DecIndexes;
    std::vector<float> m_DecVertices;

};
void ViewWindow::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
    if(releaseEvent->button() == Qt::LeftButton)
    {
        m_Rotate = false;
        m_UpdateRender = false;
    }
}

void ViewWindow::mouseMoveEvent(QMouseEvent *eventMove)
{
    if(m_Rotate){
        QVector2D vec( eventMove->pos().x() - m_LastPos.x(), eventMove->pos().y() - m_LastPos.y());

        m_MVPMatrix.rotate(m_RotateSpeed * vec.x(), QVector3D(0.f, 1.f, 0.f));
        m_MVPMatrix.rotate(m_RotateSpeed * vec.y(), QVector3D(1.f, 0.f, 0.f));
        m_LastPos = eventMove->pos();
        m_UpdateRender = true;
    }
}

void ViewWindow::mousePressEvent(QMouseEvent *eventPress)
{
    if(eventPress->button() == Qt::LeftButton){
        m_Rotate = true;
    }
    if(eventPress->button() == Qt::RightButton){
        m_RotateSpeed += 0.2f;
    }
    m_LastPos = eventPress->pos();
}
void ViewWindow::wheelEvent(QWheelEvent *event)
{
    const int degrees = event->delta() / 8;
    const int steps = degrees / 15;

    if(steps > 0) m_ZoomFactor += 0.3f;
    else m_ZoomFactor -= 0.3f;

    m_UpdateRender = true;
}

void ViewWindow::initialize()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_AngleX = m_AngleY = 0.f;
    m_RotateSpeed = 0.1f;
    srand(time(NULL));
    m_UpdateRender = true;
    mesh.load("data/cube.obj");
    std::cout << "Model loaded : " << std::endl;
    std::cout << mesh.vertices.size() << " vertices" << std::endl;
    std::cout << mesh.edges.size() << " edges" << std::endl;
    std::cout << mesh.faces.size() << " faces" << std::endl;

    m_Data = new float6[mesh.faces.size()*3];

    std::vector<Edge*> boundaries;
    std::vector<Edge> supManifold;
    Quality::extractBoudaries(mesh.edges, boundaries, supManifold);

    std::cout << boundaries.size() << " bords; " << supManifold.size() << " non 2Variétés." << std::endl;

    for(unsigned int i = 0; i < mesh.faces.size(); ++i)
    {
        for(int v = 0; v < 3; v++)
        {
            m_Data[i*3+v].x = mesh.faces[i]->vertices[v]->x;
            m_Data[i*3+v].y = mesh.faces[i]->vertices[v]->y;
            m_Data[i*3+v].z = mesh.faces[i]->vertices[v]->z;
            m_Data[i*3+v].r = mesh.faces[i]->vertices[v]->r;
            m_Data[i*3+v].g = mesh.faces[i]->vertices[v]->g;
            m_Data[i*3+v].b = mesh.faces[i]->vertices[v]->b;
        }
    }

    // Display some metric : various aspect ratios and degree
    float metricMean = 0.0, metricSD = 0.0, metricMin = 0.0, metricMax = 0.0;
    int fl = 10;
    std::cout.precision(4);
    std::cout.setf(std::ios::fixed, std:: ios::floatfield);

    std::cout << std::setw(fl) << "Metric" << " | " << std::setw(fl) << "Mean" << " | " << std::setw(fl) << "SD" << " | " << std::setw(fl) << "Min" << " | " << std::setw(fl) << "Max" << std::endl;
    Quality::getAspectRatio1(mesh.faces, metricMean, metricSD, metricMin, metricMax);
    std::cout << std::setw(fl) << "AR1" << " | " << std::setw(fl) << metricMean << " | " << std::setw(fl) << metricSD << " | " << std::setw(fl) << metricMin << " | " << std::setw(fl) << metricMax << std::endl;
    Quality::getAspectRatio2(mesh.faces, metricMean, metricSD, metricMin, metricMax);
    std::cout << std::setw(fl) << "AR2" << " | " << std::setw(fl) << metricMean << " | " << std::setw(fl) << metricSD << " | " << std::setw(fl) << metricMin << " | " << std::setw(fl) << metricMax << std::endl;
    Quality::getAspectRatio3(mesh.faces, metricMean, metricSD, metricMin, metricMax);
    std::cout << std::setw(fl) << "AR3" << " | " << std::setw(fl) << metricMean << " | " << std::setw(fl) << metricSD << " | " << std::setw(fl) << metricMin << " | " << std::setw(fl) << metricMax << std::endl;
    Quality::getDegree(mesh.vertices, metricMean, metricSD, metricMin, metricMax);
    std::cout << std::setw(fl) << "Degree" << " | " << std::setw(fl) << metricMean << " | " << std::setw(fl) << metricSD << " | " << std::setw(fl) << metricMin << " | " << std::setw(fl) << metricMax << std::endl;

    // Display holes data
    int holes = Quality::getHoleCount(boundaries);
    std::cout << "Holes : " << holes << std::endl;

    Quality::normaleComputation(mesh.faces);
    std::vector<Face*>::iterator beg = mesh.faces.begin();
    std::vector<Face*>::iterator end = mesh.faces.end();
    for(; beg != end; ++beg){
        std::cout << "Normale x  : " << (*beg)->nx << " ; Normale y : " << (*beg)->ny << " ; Normale z : " << (*beg)->nz << std::endl;
    }
    std::cout << "Beginning" << std::endl;
    Quality::MWRELR(mesh.vertices);
    std::cout << "Done !" << std::endl;

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float6) * 3 * mesh.faces.size(), m_Data, GL_STATIC_DRAW);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    m_program->link();

    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    m_zoomUniform = m_program->uniformLocation("zoomFactor");


    m_lookAt = QVector3D(0.f, 0.f, 5.f);
    m_MVPMatrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 1000.0f);
    m_MVPMatrix.lookAt(m_lookAt, QVector3D(0.f,0.f,0.f), QVector3D(0.f,1.f,0.f));
    m_MVPMatrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), 45.f));

    m_ZoomFactor = 1.f;
}


const std::string ViewWindow::vertexShaderSource =
    "#version 330\n \
    in vec3 posAttr;\n \
    in vec3 colAttr;\n \
    uniform float zoomFactor;\n\
    out vec3 col;\n \
    uniform mat4 matrix;\n \
    void main() { \n\
       col = colAttr;\n \
       gl_Position = matrix * vec4(posAttr*zoomFactor, 1);\n \
    }";

const std::string ViewWindow::fragmentShaderSource =
    "#version 330\n \
    in vec3 col;\n \
    out vec4 color;\n \
    void main() { \n\
        color = vec4(col,1);\n \
    }";

void ViewWindow::render()
{
    if(m_UpdateRender){
        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->bind();

        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        m_program->enableAttributeArray("posAttr");
        quintptr offset = 0;
        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, sizeof(float6), (const void*)offset);
        m_program->enableAttributeArray("colAttr");
        offset += sizeof(float)*3;
        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, sizeof(float6), (const void*)offset);
        m_program->setUniformValue(m_matrixUniform, m_MVPMatrix);
        m_program->setUniformValue(m_zoomUniform, m_ZoomFactor);
        glDrawArrays(GL_TRIANGLES, 0, mesh.faces.size()*3);

        m_program->disableAttributeArray("colAttr");
        m_program->disableAttributeArray("posAttr");
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_program->release();
    }


    ++m_frame;
}
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QSurfaceFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setSamples(16);

    QSurfaceFormat format;
    format.setSamples(16);

    ViewWindow window;
    window.setFormat(glFormat);
    window.resize(640, 480);
    window.show();
    window.setAnimating(true);
    return app.exec();
}

void ViewWindow::updatePoints(bool drawDecVertices)
{
    float scaleFactor = 0.8f;
    size_t decVertSize = m_DecIndexes.size();
    std::cout << "m_DecIndexes.size() = " << m_DecIndexes.size() << std::endl;
    size_t vertSize = m_points.size() / 6;

    if(drawDecVertices){
        unsigned int i = 0;
        for(; i < decVertSize; ++i)
        {
            //std::cout << "i : " << i << std::endl;
            vertices[i*3+0] = m_DecVertices[i*6+0]*scaleFactor - mx;
            vertices[i*3+1] = m_DecVertices[i*6+1]*scaleFactor - my;
            vertices[i*3+2] = m_DecVertices[i*6+2]*scaleFactor - mz;
        }
        for(; i < pointsCount; ++i){
            vertices[i*3+0] = 0.f;
            vertices[i*3+1] = 0.f;
            vertices[i*3+2] = 0.f;
        }
        pointsCount = decVertSize;
        std::cout << "Data updated according to the decimation" << std::endl;
    } else {
        pointsCount = m_points.size() / 6;
        for(unsigned int i = 0; i < pointsCount; ++i){
            vertices[i*3+0] = m_points[i*6+0]*scaleFactor - mx;
            vertices[i*3+1] = m_points[i*6+1]*scaleFactor - my;
            vertices[i*3+2] = m_points[i*6+2]*scaleFactor - mz;
        }
    }
}

void ViewWindow::loadASC(const std::string& filename, ASC& result)
{
    std::ifstream file(filename.c_str());
    if( !file.is_open() ) std::cerr << "Error : cannot open file : " << filename << std::endl;

    pointsCount = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
    result.resize(pointsCount * 6);
    file.seekg (0, file.beg);

    unsigned int i = 0;
    float progress = 0.f;

    float x = 0.f, y = 0.f, z = 0.f;
    float nx = 0.f, ny = 0.f, nz = 0.f;
    std::istringstream linestream;

    for(std::string line; getline(file, line);)
    {
        // Empty line and comments are ignored
        if(line[0] == '#' || line.size() <= 0)
            continue;

        linestream.str(line);
        linestream.seekg(0);

        // Extract position
        x = 0.f;
        y = 0.f;
        z = 0.f;
        linestream >> x >> y >> z;
        mx += x;
        my += y;
        mz += z;

        // Extract normal
        nx = 0.f;
        ny = 0.f;
        nz = 0.f;
        //linestream >> nx >> ny >> nz;

        // Fill the vertex and push it in the array
        result[i*6+0] = x;  result[i*6+1] = y;  result[i*6+2] = z;
        result[i*6+3] = nx; result[i*6+4] = ny; result[i*6+5] = nz;
        ++i;

        if((float) i / (float) pointsCount >= progress+0.1f)
        {
            progress += 0.1f;
            std::cout << "loading ... " << (progress*100) << "%" << std::endl;
        }

    }
    mx /= (float) pointsCount;
    my /= (float) pointsCount;
    mz /= (float) pointsCount;
    std::cout << "loaded " << pointsCount << " vertices" << std::endl;
    std::cout << "center is  " << mx << ", " << my << ", " << mz << std::endl;
}
