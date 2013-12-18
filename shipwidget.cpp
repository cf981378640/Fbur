/****************************************************************************
**
**20131216修改
**
****************************************************************************/
#include <iostream>
#include "shipwidget.h"

#include <QMouseEvent>
#include <QColorDialog>
#include <math.h>

ShipWidget::ShipWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    mode=1;

    rotationX = -21.0;
    rotationY = -57.0;
    rotationZ = 30.0;
    sscale = 0.5;
    faceColors[0] = Qt::red;
    faceColors[1] = Qt::green;
    faceColors[2] = Qt::blue;
    faceColors[3] = Qt::yellow;
}

void ShipWidget::initializeGL()
{
    qglClearColor(Qt::white);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void ShipWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
//    glFrustum(-x, +x, -1.0, +1.0, 4.0, 25.0);
    glOrtho(-x, +x, -1.0, +1.0, 4.0, 25.0);
    glMatrixMode(GL_MODELVIEW);
}

void ShipWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    glScalef(sscale,sscale,sscale);
    glShadeModel(GL_SMOOTH);

    glColor3f(0.0f,0.0f,0.0f);

    int modee;
    if(vPoints.empty())modee=-1;
    else modee=mode;
    switch(modee)
    {
    case 1:drawLinesPlan();break;
    case 2:drawHyCur();break;
    default:drawDefault();
    }
}

void ShipWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void ShipWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += -360 * dy;
        rotationY += -360 * dx;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        rotationX += -360 * dy;
        rotationZ += -360 * dx;
        updateGL();
    }
    lastPos = event->pos();
}


void ShipWidget::wheelEvent(QWheelEvent *event)
{
    GLfloat dd=event->delta();
    sscale+=dd/1000;
    updateGL();
}

void ShipWidget::drawDefault()
{
      QFont font("楷体",30);
      renderText(200,100,"请导入数据文件! ",font);
}



void ShipWidget::drawLinesPlan()
{
    if(vPoints.empty())return;

    qglColor(Qt::blue);
    for (sPoint p : vPoints)
    {
        double x=-1;
        double y=-1;
        double z=-1;
      if(p.x!=x){
        vector<sPoint>sP=drawXZ(p.x);
        glBegin(GL_LINE_STRIP);
        for (sPoint p : sP)
        {
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,GLfloat(p.y)/Lpp*5);
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (sPoint p : sP)
        {
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,-GLfloat(p.y)/Lpp*5);
        }
        glEnd();

        x=p.x;
    }
        if(p.z!=z){
        vector<sPoint>sP=drawZX(p.z);
        glBegin(GL_LINE_STRIP);
        for (sPoint p : sP)
        {
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,GLfloat(p.y)/Lpp*5);
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (sPoint p : sP)
        {
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,-GLfloat(p.y)/Lpp*5);
//            cerr<<GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5<<","<<GLfloat(p.z)/Lpp*5<<","<<-GLfloat(p.y)/Lpp*5;
        }
        glEnd();

        z=p.z;
    }
/*
        if(p.y!=y){
        vector<sPoint>sP=drawYZ(p);
        glBegin(GL_LINE_STRIP);
        for (sPoint p : sP)
        {
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,GLfloat(p.y)/Lpp*5);
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (sPoint p : sP)
        {
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,-GLfloat(p.y)/Lpp*5);
        }
        glEnd();

        y=p.y;
    }
*/
    }
/*    for(int j=0;j<vPoints.size();j++)
    {
        sPoint p;
        p.x=vPoints.at(j).x;
        p.y=vPoints.at(j).y;
        p.z=vPoints.at(j).z;
        glBegin(GL_POINTS);
            p.x=vPoints.at(j).x;
            p.y=vPoints.at(j).y;
            p.z=vPoints.at(j).z;
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,GLfloat(p.y)/Lpp*5);
            glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,-GLfloat(p.y)/Lpp*5);
//            cerr<<"\np.x:"<<GLfloat(p.x*deltaL- deltaL*ma)/Lpp<<"\tp.y:"<<GLfloat(p.z)/Lpp<<"\tp.z:"<<GLfloat(p.y)/Lpp;
        if(p.x!=xx)glEnd();
        xx=p.x;
//        cerr<<"\ndeltaL:"<<deltaL<<"m:"<<m<<"ma:"<<ma;
    }
*/
}

void ShipWidget::drawHyCur()
{

     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glTranslatef(-1.0, -1.0, -10.0);

     glScalef(sscale,sscale,sscale);


    qglColor(Qt::blue);
    drawAxes();
    drawZb();
    drawAw();
    drawTPC();
    drawXf();
    drawXb();
    drawVolume();

    qglColor(Qt::black);
    QFont font("楷体",40);
    renderText(100,100,"静水力曲线! ",font);
}
void ShipWidget::drawAxes()
{

   glBegin(GL_LINES);
   glVertex3f(0.0,0.0,0.0);
   glVertex3f(4.0,0.0,0.0);

   glVertex3f(0.0,0.0,0.0);
   glVertex3f(0.0,4.0,0.0);

   glEnd();
}
void ShipWidget::drawZb()
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vZb)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value,p.z,-1);
        cerr<<"Xf静水力曲线"<<p.value<<","<<p.z<<endl;
    }

    glEnd();
}
void ShipWidget::drawAw()
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vAw)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/Lpp/B*2,p.z,-1);
//        cerr<<"AW静水力曲线"<<p.value/(Lpp*B)*2<<","<<p.z<<endl;
    }

    glEnd();
}
void ShipWidget::drawTPC()
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vTPC)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value,p.z,-1);
//        cerr<<"TPC静水力曲线"<<p.value<<","<<p.z<<endl;
    }

    glEnd();
}
void ShipWidget::drawXf()           //有问题！！！！
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vXf)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value+Xm*deltaL/3,p.z,-1);
        cerr<<"TPC静水力曲线"<<p.value+Xm*deltaL/3<<","<<p.z<<endl;
    }
    glEnd();
}
void ShipWidget::drawXb()           //有问题！！！！
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vXb)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value+Xm*deltaL/3,p.z,-1);
        cerr<<"TPC静水力曲线"<<p.value+Xm*deltaL/3<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawVolume()           //有问题！！！！
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vVolume)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/Lpp/B,p.z,-1);
        cerr<<"TPC静水力曲线"<<p.value/Lpp/B<<","<<p.z<<endl;
    }
    glEnd();
}

