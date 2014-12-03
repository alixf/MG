//#include <fstream>
//#include <iostream>
//#include <sstream>
//#include <QtGui/QGuiApplication>
//#include <QtGui/QMatrix4x4>
//#include <QtGui/QOpenGLShaderProgram>
//#include <QtGui/QScreen>
//#include <QtCore/qmath.h>
//#include <QWheelEvent>
//#include <QMouseEvent>
#include <ctime>

//#include "openglwindow.hpp"
//#include "octree.hpp"
//#include "ray.hpp"
#include "vefmodel.hpp"

//typedef std::vector<float> ASC;

//struct Camera
//{
//    QVector3D position;
//    QVector3D direction;
//    QVector3D right;
//    QVector3D up;
//};

//class ViewWindow : public OpenGLWindow
//{
//public:
//    ViewWindow();
//    ~ViewWindow();
//    void initialize();
//    void render();
//    Ray rayFromCamera(int x, int y);
//    void updatePoints(bool);

//private:
//    GLuint loadShader(GLenum type, const char *source);
//    void loadASC(const std::string& filename, ASC& result);
//    void wheelEvent ( QWheelEvent * event );
//    void mouseMoveEvent ( QMouseEvent * event );

//    GLuint m_posAttr;
//    GLuint m_colAttr;
//    GLuint m_matrixUniform;
//    ASC m_points;
//    std::vector<int> indexes;
//    unsigned int pointsCount;
//    QOpenGLShaderProgram *m_program;
//    int m_frame;
//    GLfloat* vertices;
//    GLfloat* colors;
//    float m_scale;
//    float mx, my, mz;

//    Camera m_camera;
//    Octree* octree;
//    Ray m_ray;
//    std::vector<int> m_DecIndexes;
//    std::vector<float> m_DecVertices;
//};

//ViewWindow::ViewWindow()
//    : m_program(0)
//    , m_frame(0)
//    , m_scale(-0.5f)
//    , mx(0.f)
//    , my(0.f)
//    , mz(0.f)

//{
//    loadASC("sphere.asc", m_points);
//    //loadASC("Grp1-2014_Simplified.asc", m_points);

//    //std::vector<int> indexes;
//    indexes.resize(m_points.size()/6);
//    for(unsigned int i = 0; i < indexes.size(); ++i)
//        indexes[i] = i;

//    Octree octree(m_points, indexes, 10, 200, 100.f, QVector3D(mx, my, mz), 0);
//    std::cout << "nbLeaf : " << octree.getNbLeaf() << std::endl;
//    octree.decimation(m_DecIndexes, m_DecVertices, 0);

//    m_camera.position.setZ(-10.f);

//    //m_DecIndexes.resize(100);
//    //m_DecVertices.reserve(100);

//    //std::vector<float> res = octree.getNbOf(QVector3D(m_points[6*57+0], m_points[6*57+1], m_points[6*57+2]), 10.f);

//    //for(unsigned int i = 0; i < res.size(); i += 6)
//    //    std::cout << "(" << res[i+0] << " ; " << res[i+1] << " ; " << res[i+2] << ")" << std::endl;
//}

//ViewWindow::~ViewWindow()
//{
//    delete octree;
//}

////Draw decimated tree or full tree
//void ViewWindow::updatePoints(bool drawDecVertices)
//{
//    float scaleFactor = 0.8f;
//    size_t decVertSize = m_DecIndexes.size();
//    std::cout << "m_DecIndexes.size() = " << m_DecIndexes.size() << std::endl;
//    size_t vertSize = m_points.size() / 6;

//    if(drawDecVertices){
//        unsigned int i = 0;
//        for(; i < decVertSize; ++i)
//        {
//            //std::cout << "i : " << i << std::endl;
//            vertices[i*3+0] = m_DecVertices[i*6+0]*scaleFactor - mx;
//            vertices[i*3+1] = m_DecVertices[i*6+1]*scaleFactor - my;
//            vertices[i*3+2] = m_DecVertices[i*6+2]*scaleFactor - mz;
//        }
//        for(; i < pointsCount; ++i){
//            vertices[i*3+0] = 0.f;
//            vertices[i*3+1] = 0.f;
//            vertices[i*3+2] = 0.f;
//        }
//        pointsCount = decVertSize;
//        std::cout << "Data updated according to the decimation" << std::endl;
//    } else {
//        pointsCount = m_points.size() / 6;
//        for(unsigned int i = 0; i < pointsCount; ++i){
//            vertices[i*3+0] = m_points[i*6+0]*scaleFactor - mx;
//            vertices[i*3+1] = m_points[i*6+1]*scaleFactor - my;
//            vertices[i*3+2] = m_points[i*6+2]*scaleFactor - mz;
//        }
//    }
//}

//void ViewWindow::loadASC(const std::string& filename, ASC& result)
//{
//    std::ifstream file(filename.c_str());
//    if( !file.is_open() ) std::cerr << "Error : cannot open file : " << filename << std::endl;

//    pointsCount = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
//    result.resize(pointsCount * 6);
//    file.seekg (0, file.beg);

//    unsigned int i = 0;
//    float progress = 0.f;

//    float x = 0.f, y = 0.f, z = 0.f;
//    float nx = 0.f, ny = 0.f, nz = 0.f;
//    std::istringstream linestream;

//    for(std::string line; getline(file, line);)
//    {
//        // Empty line and comments are ignored
//        if(line[0] == '#' || line.size() <= 0)
//            continue;

//        linestream.str(line);
//        linestream.seekg(0);

//        // Extract position
//        x = 0.f;
//        y = 0.f;
//        z = 0.f;
//        linestream >> x >> y >> z;
//        mx += x;
//        my += y;
//        mz += z;

//        // Extract normal
//        nx = 0.f;
//        ny = 0.f;
//        nz = 0.f;
//        //linestream >> nx >> ny >> nz;

//        // Fill the vertex and push it in the array
//        result[i*6+0] = x;  result[i*6+1] = y;  result[i*6+2] = z;
//        result[i*6+3] = nx; result[i*6+4] = ny; result[i*6+5] = nz;
//        ++i;

//        if((float) i / (float) pointsCount >= progress+0.1f)
//        {
//            progress += 0.1f;
//            std::cout << "loading ... " << (progress*100) << "%" << std::endl;
//        }

//    }
//    mx /= (float) pointsCount;
//    my /= (float) pointsCount;
//    mz /= (float) pointsCount;
//    std::cout << "loaded " << pointsCount << " vertices" << std::endl;
//    std::cout << "center is  " << mx << ", " << my << ", " << mz << std::endl;
//}

//int main(int argc, char **argv)
//{
//    QGuiApplication app(argc, argv);

//    QSurfaceFormat format;
//    format.setSamples(16);

//    ViewWindow window;
//    window.setFormat(format);
//    window.resize(640, 480);
//    window.show();

//    window.setAnimating(true);

//    return app.exec();
//}

//static const char *vertexShaderSource =
//    "attribute highp vec4 posAttr;\n"
//    "attribute lowp vec4 colAttr;\n"
//    "varying lowp vec4 col;\n"
//    "uniform highp mat4 matrix;\n"
//    "void main() {\n"
//    "   col = colAttr;\n"
//    "   gl_Position = matrix * posAttr;\n"
//    "}\n";

//static const char *fragmentShaderSource =
//    "varying lowp vec4 col;\n"
//    "void main() {\n"
//    "   gl_FragColor = col;\n"
//    "}\n";

//GLuint ViewWindow::loadShader(GLenum type, const char *source)
//{
//    GLuint shader = glCreateShader(type);
//    glShaderSource(shader, 1, &source, 0);
//    glCompileShader(shader);
//    return shader;
//}

//void ViewWindow::initialize()
//{
//    m_camera.up = QVector3D(0.f, 1.f, 0.f);
//    m_camera.right = QVector3D(1.f, 0.f, 0.f);
//    m_camera.direction = QVector3D(0.f, 0.f, 1.f);

//    m_program = new QOpenGLShaderProgram(this);
//    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
//    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
//    m_program->link();
//    m_posAttr = m_program->attributeLocation("posAttr");
//    m_colAttr = m_program->attributeLocation("colAttr");
//    m_matrixUniform = m_program->uniformLocation("matrix");

//    float scaleFactor = 0.9f;
//    mx *= scaleFactor;
//    my *= scaleFactor;
//    mz *= scaleFactor;
//    vertices = new GLfloat[pointsCount*3];

//    for(unsigned int i = 0; i < pointsCount; ++i)
//    {
//        vertices[i*3+0] = m_points[i*6+0]*scaleFactor - mx;
//        vertices[i*3+1] = m_points[i*6+1]*scaleFactor - my;
//        vertices[i*3+2] = m_points[i*6+2]*scaleFactor - mz;
//    }

//    colors = new GLfloat[pointsCount*3];
//    for(unsigned int i = 0; i < pointsCount; ++i)
//    {
//        colors[i*3+1] = 1.f;
//        colors[i*3+2] = 1.f;
//        colors[i*3+0] = 1.f;
////        if (m_points[i*6+3] == 0.f &&
////            m_points[i*6+4] == 0.f &&
////            m_points[i*6+5] == 0.f)
////        if (m_DecVertices[i*6+3] == 0.f &&
////            m_DecVertices[i*6+4] == 0.f &&
////            m_DecVertices[i*6+5] == 0.f)
////        {
////            colors[i*3+1] = 1.f;
////            colors[i*3+2] = 1.f;
////            colors[i*3+0] = 1.f;
////        }
////        else
////        {
////            colors[i*3+1] = 0.5f + 0.5f * m_DecVertices[i*6+3];
////            colors[i*3+2] = 0.5f + 0.5f * m_DecVertices[i*6+4];
////            colors[i*3+0] = 0.5f + 0.5f * m_DecVertices[i*6+5];
////        }
//    }



//    updatePoints(true);
//}

//void ViewWindow::render()
//{
//    const qreal retinaScale = devicePixelRatio();
//    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

//    glClear(GL_COLOR_BUFFER_BIT);

//    m_program->bind();

//    QMatrix4x4 matrix;
//    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
//    matrix.translate(m_camera.position.x(), m_camera.position.y(), m_camera.position.z());
//    matrix.rotate(50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
//    //matrix.translate(-mx, -my, -mz);

//    //matrix.scale(m_scale);

//    m_program->setUniformValue(m_matrixUniform, matrix);

//    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
//    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);

//    glDrawArrays(GL_POINTS, 0, pointsCount);

//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);


//    m_program->release();

//    glBegin(GL_LINES);
//    glColor3f(1.0, 0.0, 0.0);
//    glVertex3f(m_ray.origin.x(), m_ray.origin.y(), m_ray.origin.z());

//    glColor3f(0.0, 1.0, 1.0);
//    glVertex3f(m_ray.origin.x() + m_ray.direction.x() * 10,
//               m_ray.origin.y() + m_ray.direction.y() * 10,
//               m_ray.origin.z() + m_ray.direction.z() * 10);
//    glEnd();

//    //glBegin(GL_LINES);
//    //glColor3f(1.0, 0.0, 0.0);
//    //glVertex3f(0,0,0);
//    //glVertex3f(10,10,10);
//    //glEnd();

//    ++m_frame;

//}
//void ViewWindow::wheelEvent( QWheelEvent * event )
//{
//    float z = (event->delta()/120) * 0.1f;
//    m_camera.position.setZ(m_camera.position.z() + z);
//}

//void ViewWindow::mouseMoveEvent(QMouseEvent* event)
//{
//    m_ray = rayFromCamera(event->x(), this->height() - event->y());
//    //QVector3D point = octree->getFirstCollision(m_ray.origin, m_ray.direction, 1.f);
//    //std::cout << point.x() << " ; " << point.y() << " ; "<< point.z() << std::endl;
//}

//Ray ViewWindow::rayFromCamera(int x, int y)
//{
//    const float width = this->width();  // pixels across
//    const float height = this->height();  // pixels high
//    double nx = (x / width) - 0.5;
//    double ny = (y / height) - 0.5;

//    QVector3D image_point = nx * m_camera.right + ny * m_camera.up + m_camera.position + m_camera.direction;
//    QVector3D ray_direction = image_point - m_camera.position;

//    Ray result;
//    result.origin = m_camera.position;
//    result.direction = ray_direction;
//    return result;
//}

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
#include <QWheelEvent>
#include <QMouseEvent>
#include "vefmodel.hpp"
#include "openglwindow.hpp"

class ViewWindow : public OpenGLWindow
{
public:
    ViewWindow(){}
    ~ViewWindow(){}
    void initialize();
    void render();

private:
    GLuint loadShader(GLenum type, const char *source)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        return shader;
    }

    static const std::string vertexShaderSource;
    static const std::string fragmentShaderSource;

    VEFModel mesh;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    float** data;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    GLuint vbo;
};

void ViewWindow::initialize()
{
    mesh.load("FantomeLite.obj");
    data = new float*[mesh.faces.size()*3];
    for(unsigned int i = 0; i < mesh.faces.size(); ++i)
    {
        for(int v = 0; v < 3; v++)
        {
            data[i*3+v] = new float[3];
            data[i*3+v][0] = mesh.faces[i].vertices[v]->x;
            data[i*3+v][1] = mesh.faces[i].vertices[v]->y;
            data[i*3+v][2] = mesh.faces[i].vertices[v]->z;
        }
    }

    const unsigned int shaderAttribute = 0;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}


const std::string ViewWindow::vertexShaderSource =
    "attribute highp vec4 posAttr; \
    attribute lowp vec4 colAttr; \
    varying lowp vec4 col; \
    uniform highp mat4 matrix; \
    void main() { \
       col = colAttr; \
       gl_Position = matrix * posAttr; \
    }";

const std::string ViewWindow::fragmentShaderSource =
    "varying lowp vec4 col; \
    void main() { \
       gl_FragColor = col; \
    }";

void ViewWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    //matrix.translate(m_camera.position.x(), m_camera.position.y(), m_camera.position.z());
    //matrix.rotate(50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    //glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    //glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, mesh.faces.size()*3);

    //glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(0);

    m_program->release();
    ++m_frame;
}
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    ViewWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();
    window.setAnimating(true);
    return app.exec();
}
