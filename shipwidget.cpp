/****************************************************************************
**
**20131218修改--二次导入清空原数据
**20131216修改
**
****************************************************************************/
#include <iostream>
#include "shipwidget.h"

#include <QMouseEvent>
#include <math.h>

ShipWidget::ShipWidget(QWidget *parent)
    : QGLWidget(parent),ship()
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    mode=1;
    lmode=-1;

    rotationX = -20.0;
    rotationY = -20.0;
    rotationZ = 5.0;
    sscale = 0.25;
}

void ShipWidget::initializeGL()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LINE_SMOOTH);
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

    switch (lmode) {
    case 0:
        break;
    default:
        break;
    case 3:
        rotationX=90;
        rotationY=rotationZ=0;
        lmode=0;
        break;
    case 2:
        rotationY=rotationX=rotationZ=0;
        lmode=0;
        break;
    case 1:
        rotationY=90;
        rotationZ=rotationX=0;
        lmode=0;
        break;
    }

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
        rotationX += -200 * dy;
        rotationY += -200 * dx;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        rotationX += -200 * dy;
        rotationZ += -200 * dx;
        updateGL();
    }
    lastPos = event->pos();
}

void ShipWidget::wheelEvent(QWheelEvent *event)
{
    GLfloat dd=event->delta();
    sscale+=dd/2000;
    updateGL();
}

void ShipWidget::drawDefault()
{
      QFont font("楷体",15);
      renderText(100,100,"请导入数据文件! ",font);
}



void ShipWidget::drawLinesPlan()
{
    if(vPoints.empty())return;

    qglColor(Qt::blue);
    for (sPoint p : vPoints)
    {
        double x=-1;
        //double y=-1;
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
        vector<sPoint>sP=drawYX(p);
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
    qglColor(Qt::black);
    QFont font("楷体",15);
    renderText(100,100,"型线图! ",font);
}

void ShipWidget::drawHyCur()
{
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glTranslatef(-2.0, -1.0, -10.0);
     glScalef(sscale,sscale*2,sscale);
     qglColor(Qt::blue);

    drawAxes();
    drawVolume();
    drawDisp();
    drawXb();
    drawZb();
    drawAw();
    drawXf();
    drawTPC();
    drawBm();
    drawBml();
    drawMTC();
    drawCwp();
    drawCm();
    drawCb();
    drawCp();

    qglColor(Qt::black);
    QFont font("楷体",15);
    renderText(100,100,"静水力曲线! ",font);
}

void ShipWidget::drawAxes()
{
   double mZ=*max_element(vZ.begin(),vZ.end());

   qglColor(Qt::gray);
   glBegin(GL_LINES);
   for(double i=0;i<Xm*2;i++)   //纵轴
   {
   glVertex3f(i,0.0,0.0);
   glVertex3f(i,mZ,0.0);
   }
   glEnd();

   glBegin(GL_LINES);
   for(double i=0;i<mZ;i++)    //横轴
   {
   glVertex3f(0.0,i,0.0);
   glVertex3f(Xm*2,i,0.0);
   }
   glEnd();
}
void ShipWidget::drawZb()
{
    //drawAw            1:1
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vZb)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value,p.z,-1);

//        cerr<<"Zb静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();

}
void ShipWidget::drawAw()
{
    //drawAw         1:100
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vAw)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/100,p.z,-1);
//        cerr<<"AW静水力曲线"<<p.value/(Lpp*B)*2<<","<<p.z<<endl;
    }

    glEnd();
}
void ShipWidget::drawTPC()
{
    //drawTPC          1:1
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
        glVertex3f((p.value+Xm*deltaL)/2.5,p.z,-1);
//        cerr<<"Xf静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}
void ShipWidget::drawXb()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vXb)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value,p.z,-1);
//        cerr<<"Xb静水力曲线"<<p.value+Xm*deltaL/3<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawVolume()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vVolume)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/50,p.z,-1);
//        cerr<<"Volume静水力曲线"<<p.value/Lpp/B<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawDisp()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vDisp)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/50,p.z,-1);
//        cerr<<"Disp静水力曲线"<<p.value/Lpp/B<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawBm()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vBm)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/2.5,p.z,-1);
//        cerr<<"Bm静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawBml()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vBml)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/2.5,p.z,-1);
//        cerr<<"Bml静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawMTC()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vMTC)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value/25,p.z,-1);
//        cerr<<"MTC静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawCwp()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vCwp)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value*10,p.z,-1);
//        cerr<<"Cwp静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawCm()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vCm)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value*10,p.z,-1);
        cerr<<"Cm静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawCb()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vCb)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value*10,p.z,-1);
//        cerr<<"Cb静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}

void ShipWidget::drawCp()           //
{
    //drawAw
    glBegin(GL_LINE_STRIP);
    int i=-1;
    for(sZValue p: vCp)
    {   i++;
        if(0==i)continue;
        glVertex3f(p.value,p.z,-1);
//        cerr<<"Cp静水力曲线"<<p.value<<","<<p.z<<endl;
    }
    glEnd();
}

