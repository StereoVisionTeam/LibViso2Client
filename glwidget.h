#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QList>

#include <GL/glu.h>
#include <GL/glut.h>


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    void paintPoints();
    QList<QList<float> >myPoints;
signals:

public slots:

    void changeRange(QString newRange);

protected:

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:

    int range;

    int xMousePos;
    int yMousePos;
    int xRot;
    int yRot;
    int zRot;

};

#endif // GLWIDGET_H
