#ifndef TGA_DECODER
#define TGA_DECODER

#include <fstream>
#include "Byte.h"
#include "ColourRGBA.h"

#pragma pack(push,1)
struct TGA_Header 
{
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)

class TGADecoder 
{
protected:
	byte* data;
	int width;
	int height;
	int bytesPerPixel;

	bool   load_rle_data(std::ifstream& in);
	bool unload_rle_data(std::ofstream& out);
public:
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};

	TGADecoder();
	TGADecoder(int w, int h, int bpp);
	TGADecoder(const TGADecoder& img);
	bool read_tga_file(const std::string filename);
	bool write_tga_file(const std::string filename, bool rle = true);
	bool flip_horizontally();
	bool flip_vertically();
	bool scale(int w, int h);
	ColourRGBA get(int x, int y);
	bool set(int x, int y, ColourRGBA& c);
	bool set(int x, int y, const ColourRGBA& c);
	~TGADecoder();
	TGADecoder& operator =(const TGADecoder& img);
	int get_width();
	int get_height();
	int get_bytesPerPixel();
	byte* buffer();
	void clear();
};

#endif