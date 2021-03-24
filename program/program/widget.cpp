#include <vector>
#include <iostream>
#include "widget.h"
#include "model.h"
#include "geometry.h"
#include "our_gl.h"

using namespace std;

int colorElem[3] = {50, 130, 200};

struct Shader : public IShader {
    Vec3f varying_intensity; // written by vertex shader, read by fragment shader

    virtual Vec4f vertex(int iface, int nthvert, Model* model, Vec3f light_dir) {
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        gl_Vertex = Viewport*Projection*ModelView*gl_Vertex;     // transform it to screen coordinates
        varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert)*light_dir); // get diffuse lighting intensity
        return gl_Vertex;
    }

    virtual bool fragment(Vec3f bar, QColor &color, bool first) {
        float intensity = varying_intensity * bar;
        if (first)
        color = QColor(255* intensity, 255* intensity, 255* intensity, 255* intensity);
        else
        {
            color = QColor(QColor::fromRgb(238, 238, 238));
            color.setAlphaF(0.002);
        }
        return false;
    }
};

void change_color(QPainter &painter, QColor color);

MWidget::MWidget(QWidget *parent): QWidget(parent)
{
    MImage = QImage(X_max, Y_max, QImage::Format_RGB32);
    MImage.fill(QColor(Qt::white));

    models.push_back(new Model("/Users/Alex/Downloads/program/obj/tree.obj"));
    models.push_back(new Model("/Users/Alex/Downloads/program/obj/fence.obj"));

    update();
}

void MWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.drawImage(0, 0, MImage);
}

void MWidget::update()
{
    QPainter painter(&MImage);

    int sizeX, sizeY;

    QColor* colorBuffer5;

    colorElem[0] = 255;
    colorElem[1] = 255;
    colorElem[2] = 255;

    float move = 0.0065;
    float move_summary = 0;
    clear_painter(painter);

    model = models[this->choose_model];
    for (int j = 0; j < 1; j++)
    {
//        if (this->choose_model == 0)
//        {
//            this->shiftFigure(-move, 'y');
//            this->shiftFigure(-(move * 1.2), 'x');
//        }
//        move_summary += move;
//        this->scaleFigure(1.02);
//        move *= 1.02;
//        start_render(painter, colorBuffer5, sizeX, sizeY, false);
//        if (j == 24)
//        {
//            if (this->choose_model == 0)
//            {
//                this->shiftFigure(move_summary, 'y');
//                this->shiftFigure(move_summary * 1.2, 'x');
//            }
//            this->scaleFigure(0.6095308705282791);
            start_render(painter, colorBuffer5, sizeX, sizeY, true);
//        }
    }

    painter.end();
}

void MWidget::start_render(QPainter &painter, QColor*& colorBuffer, int& sizeX, int& sizeY, bool first)
{
    QPen pen(QColor(255, 255, 255));
    QBrush brush(QColor(255, 255, 255));
    QColor* colorBufferTmp = new QColor[400 * 400];

    for (int i = 0; i < 400; ++i) {
        for (int j = 0; j < 400; ++j) {
            colorBufferTmp[i + j * 400] = QColor(255, 255, 255);
        }
    }

    Data data;
    data.maxX = -10000;
    data.minX = 10000;
    data.maxY = -10000;
    data.minY = 10000;

    float *zbuffer = new float[X_max*Y_max];
    for (int i=X_max*Y_max; --i; ) {
        zbuffer[i] = -std::numeric_limits<float>::max(); // shadowbuffer[i] = -std::numeric_limits<float>::max();
    }

    lookat(Vec3f(eye_x, eye_y, eye_z), center, up);
    viewport(160, 40, 650, 300);
    projection(0);
    light_dir.normalize();
    Shader shader;
    for (int i=0; i< this->model->nfaces(); i++) {
        Vec4f screen_coords[3];
        for (int j=0; j<3; j++) {
            screen_coords[j] = shader.vertex(i, j, this->model, light_dir);
        }
        triangle(screen_coords, shader, zbuffer, colorBufferTmp, data, first);
    }

    sizeX = X_max;
    sizeY = Y_max;

    colorBuffer = new QColor[sizeX * sizeY];

    sizeX = data.maxX - data.minX;
    sizeY = data.maxY - data.minY;

    pen.setColor(colorBufferTmp[0]);
    brush.setColor(colorBufferTmp[0]);

    pen.setColor(QColor::fromRgb(125, 125, 125));
    brush.setColor(QColor::fromRgb(125, 125, 125));
    painter.setBrush(brush);
    painter.setPen(pen);
    if (this->choose_model == 0)
        painter.drawEllipse(10, 90, 20, 20);
    else
        painter.drawEllipse(195, 200, 20, 20);

    for (int i = data.minX, k = 0; i < data.maxX; ++i, ++k) {
        for (int j = data.minY, m = 0; j < data.maxY; ++j, ++m) {
            pen.setColor(colorBufferTmp[i + j * 400]);
            brush.setColor(colorBufferTmp[i + j * 400]);
            painter.setBrush(brush);
            painter.setPen(pen);
            colorBuffer[k + m * sizeY] = colorBufferTmp[i + j * 400];
            if (colorBufferTmp[i + j * 400] != QColor(255, 255, 255))
            {
                painter.drawRect(i, (200 - j) + 200, 1, 1);
            }
        }
    }
}

void MWidget::clear_painter(QPainter &painter) {
    QPen pen(QColor(255, 255, 255));
    QBrush brush(QColor(255, 255, 255));
    painter.setBrush(brush);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(0, 0, 1000, 900);
}

double degreeToRadian(const double &alpha) {
    return alpha * 3.14 / 180;
}

void MWidget::findCenter() {
    std::vector<Vec3f*> verts = model->cap;
    int verts_n = verts.size();

    float maxX = verts[0]->x, maxY = verts[0]->y, maxZ = verts[0]->z;
    float minX = verts[0]->x, minY = verts[0]->y, minZ = verts[0]->z;

    for (int i = 0; i < verts_n; ++i) {
        maxX = max(maxX, verts[i]->x);
        minX = min(minX, verts[i]->x);

        maxY = max(maxY, verts[i]->y);
        minY = min(minY, verts[i]->y);

        maxZ = max(maxZ, verts[i]->z);
        minZ = min(minZ, verts[i]->z);
    }

    cntr = Vec3f((maxX + minX) / 2,
               (maxY + minY) / 2,
               (maxZ + minZ) / 2 );
}

void MWidget::rotateY3D(int i, float degree) {
    float x = model->cap[i]->x;
    float z = model->cap[i]->z;
    float cntr_x = cntr.x;
    float cntr_z = cntr.z;
    float sinAlpha = sin(degreeToRadian(degree));
    float cosAlpha = cos(degreeToRadian(degree));

    float new_x = cntr_x + (x - cntr_x) * cosAlpha - (z - cntr_z) * sinAlpha;
    float new_z = cntr_z +  (z - cntr_z) * cosAlpha + (x - cntr_x) * sinAlpha;

    model->cap[i]->x = new_x;
    model->cap[i]->z = new_z;
}

void MWidget::rotateY3Dn(int i, float degree) {
    float x = model->capn[i]->x;
    float z = model->capn[i]->z;
    float cntr_x = cntr.x;
    float cntr_z = cntr.z;
    float sinAlpha = sin(degreeToRadian(degree));
    float cosAlpha = cos(degreeToRadian(degree));

    float new_x = cntr_x + (x - cntr_x) * cosAlpha - (z - cntr_z) * sinAlpha;
    float new_z = cntr_z +  (z - cntr_z) * cosAlpha + (x - cntr_x) * sinAlpha;

    model->capn[i]->x = new_x;
    model->capn[i]->z = new_z;
}

void MWidget::rotateFigure(float degree) {
    findCenter();
    std::vector<Vec3f*> verts = model->cap;
    int verts_n = verts.size();

    for (int i = 0; i < verts_n; ++i) {
      rotateY3D(i, degree);
      rotateY3Dn(i, degree);
    }
}

void MWidget::shiftFigure(float value, char choice)
{
    findCenter();
    for (int i = 0; i < model->nverts(); ++i) {
        if (choice == 'x')
            model->cap[i]->x += value;
        else if (choice == 'y')
            model->cap[i]->y += value;
        else if (choice == 'z')
            model->cap[i]->z += value;
        }
}

float MWidget::scaleFormula(float value, float scale, float center) {
    return value * scale + (1 - scale) * center;
}

void MWidget::scaleFigure(float scale)
{
    findCenter();
    std::vector<Vec3f*> verts = model->cap;
    int verts_n = verts.size();

    for (int i = 0; i < verts_n; i++) {
        model->cap[i]->x = scaleFormula(model->cap[i]->x, scale, cntr.x);
        model->cap[i]->y = scaleFormula(model->cap[i]->y, scale, cntr.y);
        model->cap[i]->z = scaleFormula(model->cap[i]->z, scale, cntr.z);
    }
}

void MWidget::baseConfigModels()
{
    model = models[0];
    findCenter();
    float scale = 1.2;
    std::vector<Vec3f*> verts = model->cap;
    int verts_n = verts.size();

    for (int i = 0; i < verts_n; i++) {
        model->cap[i]->x = scaleFormula(model->cap[i]->x, scale, cntr.x);
        model->cap[i]->y = scaleFormula(model->cap[i]->y, scale, cntr.y);
        model->cap[i]->z = scaleFormula(model->cap[i]->z, scale, cntr.z);
    }

    model = models[1];
    findCenter();
    verts = model->cap;
    verts_n = verts.size();
    scale = 500;

    for (int i = 0; i < verts_n; i++) {
        model->cap[i]->x = scaleFormula(model->cap[i]->x, scale, cntr.x);
        model->cap[i]->y = scaleFormula(model->cap[i]->y, scale, cntr.y);
        model->cap[i]->z = scaleFormula(model->cap[i]->z, scale, cntr.z);
    }
    this->shiftFigure(0.52, 'y');
    this->shiftFigure(-0.08, 'x');
}










