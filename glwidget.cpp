#include "glwidget.h"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    range(50)
{

}
void GLWidget::mousePressEvent(QMouseEvent *event){

  xMousePos = event->pos().x();
  yMousePos = event->pos().y();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"UO";

    int dx = event->x() - xMousePos;
       int dy = event->y() - yMousePos;

       if (event->buttons() & Qt::LeftButton) {
           xRot=xRot + 0.2 * dy;
           yRot=yRot + 0.2 * dx;
       } else if (event->buttons() & Qt::RightButton) {
           xRot=xRot + 0.2 * dy;
           zRot=zRot + 0.2 * dx;
       }
       xMousePos = event->pos().x();
       yMousePos = event->pos().y();
       update();

}
//[End] Rotation with moouse


//[Begin] Painting

void GLWidget::paintGL()
{
    //Czyszczenie tła
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //Macierz modelu załadowanie
    glMatrixMode(GL_MODELVIEW);

    //Macierz modelu załadowanie jednostkowej macierzy

    glLoadIdentity();


    //Obszar renderingu
    int side = qMin(width(), height());
    glViewport((width() - side) / 2, (height() - side) / 2, side, side);


    glRotated(xRot, 1.0, 0.0, 0.0);
    glRotated(yRot, 0.0, 1.0, 0.0);
    glRotated(zRot, 0.0, 0.0, 1.0);
    //glTranslatef(0,0,15);
    glColor3f(255,0,0);
    glBegin(GL_LINES);
        glVertex3f(-25,0,0);
        glVertex3f(25,0,0);
    glEnd();

    //glLoadIdentity();

    glColor3f(0,255,0);
    glBegin(GL_LINES);
        glVertex3f(0,-25,0);
        glVertex3f(0,25,0);
    glEnd();

    //glLoadIdentity();

    glColor3f(0,0,255);
    glBegin(GL_LINES);
        glVertex3f(0,0,-25);
        glVertex3f(0,0,25);
    glEnd();

    glColor3f(0,0,0.45);

    paintPoints();

    glFlush();

    glMatrixMode( GL_PROJECTION );

    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();


    glOrtho(-1*range,range,range,-1*range,-1*range,range);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}

void GLWidget::initializeGL()
{
    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_LIGHTING );
   // glEnable( GL_LIGHT0 );
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}
void GLWidget::changeRange(QString newRange){

    qDebug()<<newRange;
    bool result= false;
    int temp=newRange.toInt(&result);
    if(result)
       range=temp;
    this->updateGL();this->update();
    //emit paintEvent(this->pa);

}

void GLWidget::paintPoints(){

  for(int i=0;i<myPoints.size();i++){
      if(myPoints[i].size() != 3)
        break;
       float x= myPoints[i][0];
       float y= myPoints[i][1];
       float z= myPoints[i][2];
       glLoadIdentity();
       glColor3f(255,255,255);
       glRotated(xRot, 1.0, 0.0, 0.0);
       glRotated(yRot, 0.0, 1.0, 0.0);
       glRotated(zRot, 0.0, 0.0, 1.0);
       glTranslatef(x,y,z);
       glutSolidSphere(0.1,10,10);
    }
}
