#ifndef __OUR_GL_H__
#define __OUR_GL_H__
#include "geometry.h"
#include "model.h"
#include <QPainter>

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;
const float depth = 2000.f;

void viewport(int x, int y, int w, int h);
void projection(float coeff=0.f); // coeff = -1/c
void lookat(Vec3f eye, Vec3f center, Vec3f up);

struct IShader {
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert, Model* model, Vec3f light_dir) = 0;
    virtual bool fragment(Vec3f bar, QColor &color, bool first) = 0;
};

struct Data {
    int minX, maxX;
    int minY, maxY;
};

void triangle(Vec4f *pts, IShader &shader, float *zbuffer, QColor *colorBuffer, Data &data, bool first);
#endif //__OUR_GL_H__
