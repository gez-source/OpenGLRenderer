#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "ColourRGBA.h"
#include "Vector2i.h"
#include "Vector3.h"
#include "Texture.h"

class Model {
private:
    std::vector<Vector3> verts_;
    std::vector<std::vector<Vector3> > faces_; // Vec3i attention, this Vec3i means vertex/uv/normal
    std::vector<Vector3> norms_;
    std::vector<Vector2> uv_;
public:
	Texture* diffusemap_;
	Texture* normalmap_;
	Texture* specularmap_;
	Texture* glowmap_;
    Texture* load_texture(std::string filename, std::string suffix);
public:
    Model(std::string filename);
    ~Model();
    int nverts();
    int nfaces();
    Vector3 normal(int iface, int nthvert);
    Vector3 normal(Vector2 uv);
    Vector3 vert(int i);
    Vector3 vert(int iface, int nthvert);
    Vector2 uv(int iface, int nthvert);
    ColourRGBA diffuse(Vector2 uv);
    float specular(Vector2 uv);
    ColourRGBA emissiveness(Vector2 uvf);
    std::vector<int> face(int idx);
};
#endif //__MODEL_H__

