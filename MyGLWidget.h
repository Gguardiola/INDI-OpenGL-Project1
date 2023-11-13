#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"

#define MUNTANYA_NUM_VERTEXS 8

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
     virtual void keyPressEvent (QKeyEvent *event);

  private:
    //-------------------------------------------------
    // Funcions generals de pintat per objectes
    void pintaUll();
    void pintaParpella();
    void pintaCap();
    void pintaMandibulaUp();
    void pintaMandibulaDown();

    //-------------------------------------------------
    //  Creació de buffers
    void creaBuffers ();        
    void creaBufferMandibula();    
    GLuint creaBufferQuadrat(glm::vec3 color);
    //-------------------------------------------------
    // Funcions de transformació pels objectes
    void transformacioMandibulaUp(double angleApertura);
    void transformacioMandibulaDown();
    void transformacioCap();
    void transformacioUll();
    void transformacioParpella(double percentatgeObert);
    //-------------------------------------------------
    void carregaShaders ();

    //-------------------------------------------------
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniforms
    GLuint transLoc;

    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO_QUADRAT_VERD, VAO_QUADRAT_BLANC, VAO_MANDIBULA, VAO_DEBUG ;
    GLint ample, alt;
    glm::vec3 COLOR_BLANC=glm::vec3(1.0,1.0,1.0);
    glm::vec3 COLOR_BLANC2=glm::vec3(200.0/255,200.0/255,126.0/255);
    glm::vec3 COLOR_VERD=glm::vec3(131.0/255,159.0/255,72.0/255);
    glm::vec3 COLOR_DEBUG=glm::vec3(1.0,1.0,0.0);
    double parpellaInput = 0.0f, jawAngle = 0.0f, moveX = 0.0f;
    int numDents = 4;

};
