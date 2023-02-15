#pragma once

#include <string>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

enum IO_MODE {READ = 0x01, WRITE = 0x02};
enum EXTENSION { PNG = 0x01, PPM = 0x02, JPG = 0x03 };

struct ImageData {
public:
	ImageData() {}
	ImageData(int imageWidth, int imageHeight, float gamma, float exposure, float aspectRatio, std::string filePath, EXTENSION fileExtension, std::string data);
	~ImageData() {}

public:
	int imageHeight;
	int imageWidth;
	float gamma; // Gamma correction
	float exposure;
	float aspectRatio;
	std::string filePath;
	EXTENSION fileExtension;
	std::string data;
};

class Image {
public:
	Image(const std::string& _filePath, IO_MODE _mode, int _resolutionWidth, int _resolutionHeight, EXTENSION _extension);

	Image(const ImageData& imageData);

	~Image();

	std::string readImage();

	void writeImage();

	void setData(std::string& strData); // Manually set pixel data without stb_image

	void setMode(IO_MODE newMode);

	unsigned char* convertStringToChar(std::string& data);

	std::string convertCharToString(unsigned char* data);

private:
	std::string filePath;
	IO_MODE mode;
	int resolutionWidth;
	int resolutionHeight;
	EXTENSION extension;
	int bytes_per_pixel;
	std::string strData;
	unsigned char* data;
};