#include <fstream>
#include <iostream>
#include <sstream>
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
#include <QWheelEvent>

#include "openglwindow.hpp"
#include "octree.hpp"

typedef std::vector<float> ASC;

class ViewWindow : public OpenGLWindow
{
public:
    ViewWindow();
    void initialize();
    void render();
    void updatePoints(bool);

private:
    GLuint loadShader(GLenum type, const char *source);
    void loadASC(const std::string& filename, ASC& result);
    void wheelEvent ( QWheelEvent * event );

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    ASC m_points;
    std::vector<int> indexes;
    unsigned int pointsCount;
    QOpenGLShaderProgram *m_program;
    int m_frame;
    GLfloat* vertices;
    GLfloat* colors;
    float m_scale;
    float mx, my, mz;
    std::vector<int> m_DecIndexes;
    std::vector<float> m_DecVertices;
};

ViewWindow::ViewWindow()
    : m_program(0)
    , m_frame(0)
    , m_scale(10.f)
    , mx(0.f)
    , my(0.f)
    , mz(0.f)

{
//    loadASC("data/Grp1-2014_Simplified.asc", m_points);
    loadASC("sphere.asc", m_points);

    //std::vector<int> indexes;
    indexes.resize(m_points.size()/6);
    for(unsigned int i = 0; i < indexes.size(); ++i)
        indexes[i] = i;

     Octree octree(m_points, indexes, 10, 200, 10.f, QVector3D(mx, my, mz), 100);
     std::cout << "nbLeaf : " << octree.getNbLeaf() << std::endl;
     octree.decimation(m_DecIndexes, m_DecVertices, 0);

     saveASC("figDecimee.asc", vertices);//m_DecVertices, octree.getNbLeaf());

    //m_DecIndexes.resize(100);
    //m_DecVertices.reserve(100);

    //std::vector<float> res = octree.getNbOf(QVector3D(m_points[6*57+0], m_points[6*57+1], m_points[6*57+2]), 10.f);

    //for(unsigned int i = 0; i < res.size(); i += 6)
    //    std::cout << "(" << res[i+0] << " ; " << res[i+1] << " ; " << res[i+2] << ")" << std::endl;
}

//Draw decimated tree or full tree
void ViewWindow::updatePoints(bool drawDecVertices)
{
    float scaleFactor = 0.9f;
    size_t decVertSize = m_DecIndexes.size();
    std::cout << "m_DecIndexes.size() = " << m_DecIndexes.size() << std::endl;
    size_t vertSize = m_points.size() / 6;

    if(drawDecVertices){
        unsigned int i = 0;
        for(; i < decVertSize; ++i)
        {
            //std::cout << "i : " << i << std::endl;
            vertices[i*3+0] = m_DecVertices[i*6+0]*scaleFactor;
            vertices[i*3+1] = m_DecVertices[i*6+1]*scaleFactor;
            vertices[i*3+2] = m_DecVertices[i*6+2]*scaleFactor;
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
            vertices[i*3+0] = m_points[i*6+0]*scaleFactor;
            vertices[i*3+1] = m_points[i*6+1]*scaleFactor;
            vertices[i*3+2] = m_points[i*6+2]*scaleFactor;
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

void ViewWindow::saveASC(const std::string& filename, std::vector<float> vertices) {

    ofstream fichier(filename.c_str(), ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

    if(fichier)  // si l'ouverture a réussi
    {
        //écrire dans le fichier
        for (int i = 0; i < vertices.size() ; ++i) {

            fichier << vertices[i] << " ";

            if (i % 6 == 0)
                fichier << endl;
        }

        fichier.close();  // on referme le fichier
    }
    else  // sinon
        cerr << "Erreur à l'ouverture !" << endl;

    return 0;
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

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

GLuint ViewWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void ViewWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    float scaleFactor = 0.9f;
    mx *= scaleFactor;
    my *= scaleFactor;
    mz *= scaleFactor;
    vertices = new GLfloat[pointsCount*3];

    for(unsigned int i = 0; i < pointsCount; ++i)
    {
        vertices[i*3+0] = m_points[i*6+0]*scaleFactor;
        vertices[i*3+1] = m_points[i*6+1]*scaleFactor;
        vertices[i*3+2] = m_points[i*6+2]*scaleFactor;

    }
    colors = new GLfloat[pointsCount*3];
    for(unsigned int i = 0; i < pointsCount; ++i)
    {
        colors[i*3+1] = 1.f;
        colors[i*3+2] = 1.f;
        colors[i*3+0] = 1.f;
//        if (m_points[i*6+3] == 0.f &&
//            m_points[i*6+4] == 0.f &&
//            m_points[i*6+5] == 0.f)
//        if (m_DecVertices[i*6+3] == 0.f &&
//            m_DecVertices[i*6+4] == 0.f &&
//            m_DecVertices[i*6+5] == 0.f)
//        {
//            colors[i*3+1] = 1.f;
//            colors[i*3+2] = 1.f;
//            colors[i*3+0] = 1.f;
//        }
//        else
//        {
//            colors[i*3+1] = 0.5f + 0.5f * m_DecVertices[i*6+3];
//            colors[i*3+2] = 0.5f + 0.5f * m_DecVertices[i*6+4];
//            colors[i*3+0] = 0.5f + 0.5f * m_DecVertices[i*6+5];
//        }
    }



    updatePoints(true);
}

void ViewWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);

    matrix.translate(0, 0, -m_scale);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    matrix.translate(-mx, -my, -mz);



    //matrix.scale(m_scale);

    m_program->setUniformValue(m_matrixUniform, matrix);

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, pointsCount);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;

}
void ViewWindow::wheelEvent( QWheelEvent * event )
{
    m_scale += (event->delta()/120) * 0.1f;
    std::cout << m_scale << std::endl;
}
