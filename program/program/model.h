#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vec3f> norms_;

public:
    Model(const char *filename);
    ~Model();

    int nverts();
    int nfaces();

    Vec3f normal(int iface, int nthvert);

    Vec3f vert(int i);
    Vec3f vert(int iface, int nthvert);

    std::vector<int> face(int idx);

    std::vector<Vec3f*> cap;
    std::vector<Vec3f*> capn;
};
#endif //__MODEL_H__

