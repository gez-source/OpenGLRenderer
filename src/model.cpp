#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"
#include "WavefrontOBJMeshImporter.h"

Model::Model(std::string filename) : verts_(), faces_(), norms_(), uv_(), diffusemap_(), normalmap_(), specularmap_(), glowmap_()
{
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;

    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vector3 v;

			iss >> v.x;
			iss >> v.y;
			iss >> v.z;

			verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vn "))
        {
            iss >> trash >> trash;
            Vector3 n;

			iss >> n.x;
			iss >> n.y;
			iss >> n.z;

            norms_.push_back(n);
        }
        else if (!line.compare(0, 3, "vt "))
        {
            iss >> trash >> trash;
            Vector2 uv;

			iss >> uv.x;
			iss >> uv.y;

            uv_.push_back(uv);
        }
        else if (!line.compare(0, 2, "f "))
        {
			std::vector<Vector3> f;
			Vector3 tmp;
			iss >> trash;

            while (iss >> tmp.x >> trash >> tmp.y >> trash >> tmp.z)
            {
				// in wavefront obj all indices start at 1, not zero
				tmp.x = tmp.x - 1.0f;
				tmp.y = tmp.y - 1.0f;
				tmp.z = tmp.z - 1.0f;

                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
	diffusemap_ = load_texture(filename, "_diffuse.tga");
	normalmap_ = load_texture(filename, "_nm_tangent.tga");
	specularmap_ = load_texture(filename, "_spec.tga");
	glowmap_ = load_texture(filename, "_glow.tga");
}

Model::~Model()
{

}

int Model::nverts()
{
    return (int)verts_.size();
}

int Model::nfaces()
{
    return (int)faces_.size();
}

std::vector<int> Model::face(int iface)
{
    std::vector<int> faces;
	std::vector<Vector3> face;
	int vertex;
    for (int i=0; i<(int)faces_[iface].size(); i++)
    {
		face = faces_[iface];
		Vector3 vertex = face[i];
		int positionIndex = (int)vertex[0];
		faces.push_back(positionIndex);
    }
    return faces;
}

Vector3 Model::vert(int i)
{
    return verts_[i];
}

Vector3 Model::vert(int iface, int nthvert)
{
	std::vector<Vector3> face = faces_[iface];
	Vector3 vertex = face[nthvert];
	int positionIndex = (int)vertex[0];
	Vector3 position = verts_[positionIndex];
	return position;
}

Texture* Model::load_texture(std::string filename, std::string suffix)
{
	Texture* texture = nullptr;
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos)
    {
        texfile = texfile.substr(0,dot) + suffix;

		texture = Texture::LoadTexture(texfile).Anisotropic()->Create();
    }
	return texture;
}

ColourRGBA Model::diffuse(Vector2 uvf)
{
    ColourRGBA rgba = diffusemap_->textureData->get(uvf[0], uvf[1]);
    return rgba;
}

Vector3 Model::normal(Vector2 uvf)
{
	ColourRGBA c = normalmap_->textureData->get(uvf[0], uvf[1]);
    Vector3 normal;

	normal.z = c.b * 2.f - 1.f;
	normal.y = c.r * 2.f - 1.f;
	normal.x = c.g * 2.f - 1.f;

    return normal;
}

Vector2 Model::uv(int iface, int nthvert)
{
	std::vector<Vector3> face = faces_[iface];
	Vector3 vertex = face[nthvert];
	int texCoordIndex = (int)vertex[1];
	Vector2 texCoord = uv_[texCoordIndex];
    return texCoord;
}

float Model::specular(Vector2 uvf)
{
    return specularmap_->textureData->get(uvf[0], uvf[1]).r / 1.0f;
}

ColourRGBA Model::emissiveness(Vector2 uvf)
{
    ColourRGBA rgba = glowmap_->textureData->get(uvf[0], uvf[1]);
    return rgba;
}

Vector3 Model::normal(int iface, int nthvert)
{
	std::vector<Vector3> face = faces_[iface];
	Vector3 vertex = face[nthvert];
	int normalIndex = (int)vertex[2];
	return norms_[normalIndex].Normalised();
}