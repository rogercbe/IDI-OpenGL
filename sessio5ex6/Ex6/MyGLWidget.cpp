#include <GL/glew.h>
#include "MyGLWidget.h"
#include <iostream>
#define GLM_FORCE_RADIANS
using namespace std;

#include <iostream>

float maxy = 0;
float miny = 0;
float maxz = 0;
float minz = 0;
float maxx = 0;
float minx = 0;

MyGLWidget::MyGLWidget (QGLFormat &f, QWidget* parent) : QGLWidget(f, parent)
{
  setFocusPolicy(Qt::ClickFocus); // per rebre events de teclat
  scale = 1.0f;
  euld = 0.0f;
  eulp = 0.0f;
  escala = 0.0f;
}

void MyGLWidget::initializeGL () 
{
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  patr.load("models/Patricio.obj");
  carregaShaders();
  createBuffers();
  //modelTransform2 ();
  modelTransform1();
  viewTransform();
  projectTransform();

  // Activem Z-Buffer
  glEnable(GL_DEPTH_TEST);

}

void MyGLWidget::paintGL () 
{



  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra
  glBindVertexArray (VAO_Terra);

  modelTransform2();

  // pintem
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Activem el VAO per a pintar la caseta
  glBindVertexArray (VAO_Patricio);

  modelTransform1();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size() * 3);

  glBindVertexArray (VAO_Patricio);

  modelTransform3();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size() * 3);



  glBindVertexArray (0);

}

void MyGLWidget::modelTransform1 ()
{
  // Matriu de transformació de model
  glm::mat4 transform;
  transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0,0.5,1.0));
  transform = glm::scale(transform, glm::vec3(escala,escala,escala));
  transform = glm::translate(transform, glm::vec3(-centrePatr[0],-centrePatr[1],-centrePatr[2]));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}

void MyGLWidget::modelTransform2 ()
{
  // Matriu de transformació de model
  glm::mat4 transform;
  transform = glm::mat4(1.f);
 // transform = glm::rotate(transform, .58f, glm::vec3(1.,0.,0.));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform3 ()
{
    glm::mat4 transform;
    transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0,0.5,-1.0));
    transform = glm::rotate(transform, float(M_PI), glm::vec3(0.0,1.0,0.0));
    transform = glm::scale(transform, glm::vec3(escala,escala,escala));
    transform = glm::translate(transform, glm::vec3(-centrePatr[0],-centrePatr[1],-centrePatr[2]));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{

    if(w >= h){
        float x = (w - h)/2;
        glViewport(x, 0 , h, h);
    } else {
        float y = (h - w)/2;
        glViewport(0, y, w, w);
    }
}


void MyGLWidget::createBuffers ()
{


  // Dades del Homero
  // Dos VBOs, un amb posició i l'altre amb color

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Patricio);
  glBindVertexArray(VAO_Patricio);

  glGenBuffers(1, &VBO_PatricioPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * patr.faces().size() * 3 * 3, patr.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_PatricioCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * patr.faces().size() * 3 * 3, patr.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  calculaCapsaModel();


  // Dades del terra
  // Dos VBOs, un amb posició i l'altre amb color
  glm::vec3 posterra[4] = {
    glm::vec3(-2.0, 0.0, -2.0),
    glm::vec3(-2.0, 0.0, 2.0),
    glm::vec3(2.0, 0.0, -2.0),
    glm::vec3(2.0, 0.0, 2.0)
  }; 
  glm::vec3 colterra[4] = {
    glm::vec3(1,1,0),
    glm::vec3(1,1,0),
    glm::vec3(1,0,1),
    glm::vec3(1,0,1)
  };

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}


void MyGLWidget::carregaShaders() 
{
  // Creem els shaders per al fragment shader i el vertex shader
  QGLShader fs (QGLShader::Fragment, this);
  QGLShader vs (QGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

void MyGLWidget::calculaRadi(){

    float dx = maxx - centrePatr[0];
    float dy = maxy - centrePatr[1];
    float dz = maxz - centrePatr[2];

    radi = (dx * dx) + (dy * dy) + (dz * dz);
    radi = double(sqrt(radi));
    distancia = radi*2;
}

void MyGLWidget::projectTransform(){

    double catet = radi/(distancia);
    angle = asin(catet);
    glm::mat4 Proj = glm::perspective(2 * angle , 1.0, radi, 3*radi);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(){


    calculaRadi();


    //glm::mat4 View = glm::lookAt(glm::vec3(0,0,2 * radi),
    //                             glm::vec3(0,0,0),
    //                             glm::vec3(0,1,0));

    glm::mat4 View;
    View = glm::translate(View, glm::vec3(0.0,0.0,-2*radi));
    View = glm::rotate(View,euld + angleX, glm::vec3(1.0,0.0,0.0));
    View = glm::rotate(View,-eulp + angleY, glm::vec3(0.0,1.0,0.0));
    View = glm::translate(View, glm::vec3(-0.0,-0.0,-0.0));


    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  //float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)
      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escala = 1.0/(maxy-miny);
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = (miny+maxy)/2.0; centrePatr[2] = (minz+maxz)/2.0;

}


void MyGLWidget::keyPressEvent (QKeyEvent *e)
{
  switch (e->key())
  {
    case Qt::Key_Escape:
        exit(0);

    case Qt::Key_Left:
        angleX -= 0.2;
        viewTransform();
        break;

    case Qt::Key_Right:
        angleX += 0.2;
        viewTransform();
        break;

    case Qt::Key_Up:
      angleY -= 0.2;
      viewTransform();
      break;
    case Qt::Key_Down:
      angleY += 0.2;
      viewTransform();
      break;
    case Qt::Key_Z:
        distancia -= 0.2;
        projectTransform();
        break;
    case Qt::Key_X:
        distancia += 0.2;
        projectTransform();
      break;

    default: e->ignore(); break;
  }
  updateGL();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    angleX += (e->y() - yClick) * M_PI / 180.0;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  updateGL ();
}

