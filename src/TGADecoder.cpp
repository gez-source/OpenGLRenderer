#include "TGADecoder.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
//#include <math.h>

TGADecoder::TGADecoder() : data(nullptr), width(0), height(0), bytesPerPixel(0) {}

TGADecoder::TGADecoder(int w, int h, int bpp) : data(nullptr), width(w), height(h), bytesPerPixel(bpp) {
	unsigned long nbytes = width * height * bytesPerPixel;
	data = new byte[nbytes];
	memset(data, 0, nbytes);
}

TGADecoder::TGADecoder(const TGADecoder& img) : data(nullptr), width(img.width), height(img.height), bytesPerPixel(img.bytesPerPixel) {
	unsigned long nbytes = width * height * bytesPerPixel;
	data = new byte[nbytes];
	memcpy(data, img.data, nbytes);
}

TGADecoder::~TGADecoder() {
	if (data) delete[] data;
}

TGADecoder& TGADecoder::operator =(const TGADecoder& img) {
	if (this != &img) {
		if (data) delete[] data;
		width = img.width;
		height = img.height;
		bytesPerPixel = img.bytesPerPixel;
		unsigned long nbytes = width * height * bytesPerPixel;
		data = new byte[nbytes];
		memcpy(data, img.data, nbytes);
	}
	return *this;
}

bool TGADecoder::read_tga_file(const std::string filename) {
	if (data) delete[] data;
	data = nullptr;
	std::ifstream in;
	in.open(filename.c_str(), std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		in.close();
		return false;
	}
	TGA_Header header;
	in.read((char*)& header, sizeof(header));
	if (!in.good()) {
		in.close();
		std::cerr << "an error occured while reading the header\n";
		return false;
	}
	width = header.width;
	height = header.height;
	bytesPerPixel = header.bitsperpixel >> 3;
	if (width <= 0 || height <= 0 || (bytesPerPixel != GRAYSCALE && bytesPerPixel != RGB && bytesPerPixel != RGBA)) {
		in.close();
		std::cerr << "bad bpp (or width/height) value\n";
		return false;
	}
	unsigned long nbytes = bytesPerPixel * width * height;
	data = new byte[nbytes];
	if (3 == header.datatypecode || 2 == header.datatypecode) {
		in.read((char*)data, nbytes);
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else if (10 == header.datatypecode || 11 == header.datatypecode) {
		if (!load_rle_data(in)) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else {
		in.close();
		std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
		return false;
	}
	if (!(header.imagedescriptor & 0x20)) {
		flip_vertically();
	}
	if (header.imagedescriptor & 0x10) {
		flip_horizontally();
	}
	std::cerr << width << "x" << height << "/" << bytesPerPixel * 8 << "\n";
	in.close();
	return true;
}

bool TGADecoder::load_rle_data(std::ifstream& in) {
	unsigned long pixelcount = width * height;
	unsigned long currentpixel = 0;
	unsigned long currentbyte = 0;
	byte* colorbuffer = new byte[4]; // bgra

	do {
		byte chunkheader = 0;
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader < 128) {
			chunkheader++;
			for (int i = 0; i < chunkheader; i++) {
				in.read((char*)colorbuffer, bytesPerPixel);
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int t = 0; t < bytesPerPixel; t++)
					data[currentbyte++] = colorbuffer[t];
				currentpixel++;
				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
		else {
			chunkheader -= 127;
			in.read((char*)colorbuffer, bytesPerPixel);
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}
			for (int i = 0; i < chunkheader; i++) {
				for (int t = 0; t < bytesPerPixel; t++)
					data[currentbyte++] = colorbuffer[t];
				currentpixel++;
				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);

	delete[] colorbuffer;

	return true;
}

bool TGADecoder::write_tga_file(const std::string filename, bool rle) {
	byte developer_area_ref[4] = { 0, 0, 0, 0 };
	byte extension_area_ref[4] = { 0, 0, 0, 0 };
	byte footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };
	std::ofstream out;
	out.open(filename.c_str(), std::ios::binary);
	if (!out.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		out.close();
		return false;
	}
	TGA_Header header;
	memset((void*)& header, 0, sizeof(header));
	header.bitsperpixel = bytesPerPixel << 3;
	header.width = width;
	header.height = height;
	header.datatypecode = (bytesPerPixel == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
	header.imagedescriptor = 0x20; // top-left origin
	out.write((char*)& header, sizeof(header));
	if (!out.good()) {
		out.close();
		std::cerr << "can't dump the tga file\n";
		return false;
	}
	if (!rle) {
		out.write((char*)data, width * height * bytesPerPixel);
		if (!out.good()) {
			std::cerr << "can't unload raw data\n";
			out.close();
			return false;
		}
	}
	else {
		if (!unload_rle_data(out)) {
			out.close();
			std::cerr << "can't unload rle data\n";
			return false;
		}
	}
	out.write((char*)developer_area_ref, sizeof(developer_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char*)extension_area_ref, sizeof(extension_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char*)footer, sizeof(footer));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.close();
	return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
bool TGADecoder::unload_rle_data(std::ofstream& out) {
	const byte max_chunk_length = 128;
	unsigned long npixels = width * height;
	unsigned long curpix = 0;
	while (curpix < npixels) {
		unsigned long chunkstart = curpix * bytesPerPixel;
		unsigned long curbyte = curpix * bytesPerPixel;
		byte run_length = 1;
		bool raw = true;
		while (curpix + run_length < npixels && run_length < max_chunk_length) {
			bool succ_eq = true;
			for (int t = 0; succ_eq && t < bytesPerPixel; t++) {
				succ_eq = (data[curbyte + t] == data[curbyte + t + bytesPerPixel]);
			}
			curbyte += bytesPerPixel;
			if (1 == run_length) {
				raw = !succ_eq;
			}
			if (raw && succ_eq) {
				run_length--;
				break;
			}
			if (!raw && !succ_eq) {
				break;
			}
			run_length++;
		}
		curpix += run_length;
		out.put(raw ? run_length - 1 : run_length + 127);
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
		out.write((char*)(data + chunkstart), (raw ? run_length * bytesPerPixel : bytesPerPixel));
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
	}
	return true;
}

ColourRGBA TGADecoder::get(int x, int y) {
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		return ColourRGBA();
	}
	byte* col = data + (x + y * width) * bytesPerPixel; // bgra
	return ColourRGBA(col[3], col[2], col[1], col[0]);
	//return TGAColor(data + (x + y * width) * bytesPerPixel, bytesPerPixel);
}

bool TGADecoder::set(int x, int y, ColourRGBA& c) {
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		return false;
	}
	memcpy(data + (x + y * width) * bytesPerPixel, c.ToBGRA(), bytesPerPixel);
	return true;
}

bool TGADecoder::set(int x, int y, const ColourRGBA& c) {
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		return false;
	}
	memcpy(data + (x + y * width) * bytesPerPixel, c.ToBGRA(), bytesPerPixel);
	return true;
}

int TGADecoder::get_bytesPerPixel() {
	return bytesPerPixel;
}

int TGADecoder::get_width() {
	return width;
}

int TGADecoder::get_height() {
	return height;
}

bool TGADecoder::flip_horizontally() {
	if (!data) return false;
	int half = width >> 1;
	for (int i = 0; i < half; i++) {
		for (int j = 0; j < height; j++) {
			ColourRGBA c1 = get(i, j);
			ColourRGBA c2 = get(width - 1 - i, j);
			set(i, j, c2);
			set(width - 1 - i, j, c1);
		}
	}
	return true;
}

bool TGADecoder::flip_vertically() {
	if (!data) return false;
	unsigned long bytes_per_line = width * bytesPerPixel;
	byte* line = new byte[bytes_per_line];
	int half = height >> 1;
	for (int j = 0; j < half; j++) {
		unsigned long l1 = j * bytes_per_line;
		unsigned long l2 = (height - 1 - j) * bytes_per_line;
		memmove((void*)line, (void*)(data + l1), bytes_per_line);
		memmove((void*)(data + l1), (void*)(data + l2), bytes_per_line);
		memmove((void*)(data + l2), (void*)line, bytes_per_line);
	}
	delete[] line;
	return true;
}

byte* TGADecoder::buffer() {
	return data;
}

void TGADecoder::clear() {
	memset((void*)data, 0, width * height * bytesPerPixel);
}

bool TGADecoder::scale(int w, int h) {
	if (w <= 0 || h <= 0 || !data) return false;
	byte* tdata = new byte[w * h * bytesPerPixel];
	int nscanline = 0;
	int oscanline = 0;
	int erry = 0;
	unsigned long nlinebytes = w * bytesPerPixel;
	unsigned long olinebytes = width * bytesPerPixel;
	for (int j = 0; j < height; j++) {
		int errx = width - w;
		int nx = -bytesPerPixel;
		int ox = -bytesPerPixel;
		for (int i = 0; i < width; i++) {
			ox += bytesPerPixel;
			errx += w;
			while (errx >= (int)width) {
				errx -= width;
				nx += bytesPerPixel;
				memcpy(tdata + nscanline + nx, data + oscanline + ox, bytesPerPixel);
			}
		}
		erry += h;
		oscanline += olinebytes;
		while (erry >= (int)height) {
			if (erry >= (int)height << 1) // it means we jump over a scanline
				memcpy(tdata + nscanline + nlinebytes, tdata + nscanline, nlinebytes);
			erry -= height;
			nscanline += nlinebytes;
		}
	}
	delete[] data;
	data = tdata;
	width = w;
	height = h;
	return true;
}

