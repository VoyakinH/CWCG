#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QStack>
#include <QTime>
#include <QCoreApplication>

#include "model.h"
#include "geometry.h"
#include "our_gl.h"

#define X_max 400
#define X_min 0
#define Y_max 400
#define Y_min 0
#define Z_max 255
#define Z_min 0

#define INT_MIN_M -2147483647
#define INT_MAX_M  2147483647

class MWidget: public QWidget
{
    Q_OBJECT

private:
    float angle_eye = -60;
    float eye_x = cosf(angle_eye * 3.14 / 180), discard_eye_x = eye_x;
    float eye_y = 0, discard_eye_y = eye_y;
    float eye_z = sinf(angle_eye * 3.14 / 180), discard_eye_z = eye_z;
    float light_x = cosf(angle_eye * 3.14 / 180),   discard_light_x = light_x;
    float light_y = 0,   discard_light_y = light_y;
    float light_z = sinf(angle_eye * 3.14 / 180),   discard_light_z = light_z;

    bool set = 0;

    std::vector<Model*> models;
    Model *model = NULL;
    Vec3f light_dir = Vec3f(light_x, light_y, light_z);
    Vec3f up = Vec3f(0,1,0);
    Vec3f eye = Vec3f(eye_x, eye_y, eye_z);
    Vec3f center = Vec3f(0,0,0);
    Vec3f cntr = Vec3f(0,5,5);
    QImage MImage;
    bool start_initialization = 1;

public:
    explicit MWidget(QWidget *parent = nullptr);
    void start_render(QPainter &painter, QColor*& colorBuffer, int& sizeX, int& sizeY, bool first);
    void render(QPainter &painter);
    void update();

    void clear_painter(QPainter &painter);
    void findCenter();
    void rotateY3D(int i, float degree);
    void rotateY3Dn(int i, float degree);
    void rotateFigure(float degree);
    void shiftFigure(float value, char choice);
    void scaleFigure(float scale);
    float scaleFormula(float value, float scale, float center);
    int choose_model = 0;
    void baseConfigModels();
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // WIDGET_H
