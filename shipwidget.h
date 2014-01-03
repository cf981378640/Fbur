/****************************************************************************
**

**
****************************************************************************/

#ifndef ShipWidget_H
#define ShipWidget_H

#include <QGLWidget>
#include "ship.h"

class ShipWidget : public QGLWidget,public ship
{
    Q_OBJECT

public:

    ShipWidget(QWidget *parent = 0);

    int mode,lmode;
protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void drawLinesPlan();
    void drawHyCur();
    void drawBC();

    void drawDefault();

    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat sscale;
    QColor faceColors[4];
    QPoint lastPos;
    void drawAxes();
    void drawAw();
    void drawZb();
    void drawTPC();
    void drawXf();
    void drawVolume();
    void drawXb();
    void drawMTC();
    void drawBml();
    void drawBm();
    void drawDisp();
    void drawCp();
    void drawCb();
    void drawCm();
    void drawCwp();
};

#endif // ShipWidget_H
