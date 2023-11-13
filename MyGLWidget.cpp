
//#include <GL/glew.h>
#include "MyGLWidget.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat

}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (200/255.0, 220/255.0, 255/255.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();

  // Inicialització de variables d'estat
 }

void MyGLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retna.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

    glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

    pintaCap();
    pintaUll();
    pintaParpella();
    pintaMandibulaUp();
    pintaMandibulaDown();

    // Desactivem el VAO
    glBindVertexArray(0);

}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

void MyGLWidget::pintaUll(){
    transformacioUll();
    glBindVertexArray(VAO_QUADRAT_BLANC);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
void MyGLWidget::pintaParpella(){
    transformacioParpella(parpellaInput);
    glBindVertexArray(VAO_QUADRAT_VERD);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}
void MyGLWidget::pintaCap(){
    transformacioCap();
    glBindVertexArray(VAO_QUADRAT_VERD);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}
void MyGLWidget::pintaMandibulaUp(){
    transformacioMandibulaUp(jawAngle);
    glBindVertexArray(VAO_MANDIBULA);
    glDrawArrays(GL_TRIANGLES, 0, (numDents+2)*3);

}
void MyGLWidget::pintaMandibulaDown(){

    transformacioMandibulaDown();
    glBindVertexArray(VAO_MANDIBULA);
    glDrawArrays(GL_TRIANGLES, 0, (numDents+2)*3);

}


void MyGLWidget::keyPressEvent(QKeyEvent* event)
{    
    makeCurrent();

    switch (event->key()) {

        case Qt::Key_A: {
            if(jawAngle < 45.0f) jawAngle += 5.0f;
            std::cout<<jawAngle<<std::endl;
            break;
        }
        case Qt::Key_D: {
            if(jawAngle > 0.0f) jawAngle -= 5.0f;
            std::cout<<jawAngle<<std::endl;
            break;
        }
        case Qt::Key_S: {
            if(parpellaInput <= 1.0f) parpellaInput += 0.1f;
            break;
        }
        case Qt::Key_W: {
            if(parpellaInput > 0.0f) parpellaInput -= 0.1f;
            break;
        }
        case Qt::Key_Left: {
            if(jawAngle > 0.0f) jawAngle -= 5.0f;
            std::cout<<jawAngle<<std::endl;
            moveX -= 0.1f;
            break;
        }
        case Qt::Key_Right: {
            if(jawAngle < 45.0f) jawAngle += 5.0f;
            std::cout<<jawAngle<<std::endl;
            moveX += 0.1f;
            break;
        }
        default: event->ignore(); break;
    }

    update();
}

void MyGLWidget::transformacioMandibulaUp(double angleApertura){
    glm::mat4 transform (1.0f);

    transform = glm::translate(transform,glm::vec3(moveX,0.0,0.0));
    transform = glm::translate(transform,glm::vec3(-1.0/12,1.0/15,0.0));
    transform = glm::rotate(transform,glm::radians(float(angleApertura)),glm::vec3(0.0,0.0,1.0));
    transform = glm::rotate(transform,glm::radians(180.0f),glm::vec3(0.0,0.0,1.0));
    transform = glm::scale(transform,glm::vec3(2.0/3,2.0/3,0.0));
    transform = glm::translate(transform,glm::vec3(-1.0,-0.21,0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioMandibulaDown(){
    glm::mat4 transform (1.0f);

    transform = glm::translate(transform,glm::vec3(0.0+moveX,-3.0/15,0.0));
    transform = glm::scale(transform,glm::vec3(2.0/3,2.0/3,0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioCap(){
    glm::mat4 transform (1.0f);

    transform = glm::translate(transform,glm::vec3(-2.0/6+moveX,0.0,0.0));
    transform = glm::scale(transform,glm::vec3(1.0/3,1.0/3,0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::transformacioUll(){
    glm::mat4 transform (1.0f);

    transform = glm::translate(transform,glm::vec3(-3.0/9+moveX,2.0/15,0.0));
    transform = glm::scale(transform,glm::vec3(1.0/9,1.0/9,0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::transformacioParpella(double percentatgeObert){
    glm::mat4 transform (1.0f);

    std::cout<<percentatgeObert<<std::endl;
    transform = glm::translate(transform,glm::vec3(-3.0/9+moveX,((-percentatgeObert+2.2)/9),0.0));
    transform = glm::scale(transform,glm::vec3(1.0/9,(-percentatgeObert/9),0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}


void MyGLWidget::creaBuffers (){
    creaBufferMandibula();
    VAO_QUADRAT_VERD = creaBufferQuadrat(COLOR_VERD);
    VAO_QUADRAT_BLANC = creaBufferQuadrat(COLOR_BLANC);
    VAO_DEBUG = creaBufferQuadrat(glm::vec3(1.0,0.0,1.0));
}


GLuint MyGLWidget::creaBufferQuadrat(glm::vec3 color){

    GLuint vaoId;
    glm::vec3 Vertices[4];  // Tres vèrtexs amb X, Y i Z
    Vertices[0] = glm::vec3(-1.0,  1.0, 0.0);
    Vertices[1] = glm::vec3(-1.0, -1.0, 0.0);
    Vertices[2] = glm::vec3( 1.0,  1.0, 0.0);
    Vertices[3] = glm::vec3( 1.0, -1.0, 0.0);

    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glm::vec3 Colors[4];
    for(int i=0;i<4;i++) {
        Colors[i] = color;//
    }
    // Creació del buffer amb les dades dels vèrtexs
    GLuint VBO[2];
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Creació del buffer de colors
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);


    // Desactivem elr VAO
    glBindVertexArray(0);

    // retornem l'identificador de VAO creat
    return vaoId;
}

void MyGLWidget::creaBufferMandibula()
{
  float w = 1;
  float h = 0.2;
  float toothWidth = w / numDents;
  int numVertexs = (numDents+2) * 3;
  glm::vec3 Colors[numVertexs];
  glm::vec3 Vertices[numVertexs];
  glm::vec3 black(0,0,0);
  int i=0;
  Vertices[i++] = glm::vec3(0.0 , 0.0, 0.0);
  Vertices[i++] = glm::vec3(w, 0.0, 0.0);
  Vertices[i++] = glm::vec3(0.0, h, 0.0);
  Vertices[i++] = glm::vec3(w  , h, 0.0);
  Vertices[i++] = glm::vec3(0.0, h, 0.0);
  Vertices[i++] = glm::vec3(w, 0.0, 0.0);
  int jawVerts = i-1; //guarda los vertices que son mandibula verde
  float x=0;
  for(int d=0;  d<numDents; d++, x+=toothWidth) {
      Vertices[i++] = glm::vec3(x, h, 0.0);
      Vertices[i++] = glm::vec3(x+toothWidth/2, 2*h, 0.0);
      Vertices[i++] = glm::vec3(x+toothWidth, h, 0.0);
  }

  for (int i = jawVerts; i >= 0; --i) {
      Colors[i]=COLOR_VERD;
  }

  for(int i=jawVerts+1;i<numVertexs;i++) {
      if(i%3 == 1) {
          Colors[i]=COLOR_BLANC2;

      }
      else{
        Colors[i] = COLOR_BLANC;
      }

  }


  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO_MANDIBULA);
  glBindVertexArray(VAO_MANDIBULA);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO[2];
  glGenBuffers(2, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);


  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  transLoc = glGetUniformLocation(program->programId(), "TG");
}
