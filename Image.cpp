#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

ImageData::ImageData(int imageWidth, int imageHeight, float gamma, float exposure, float aspectRatio, std::string filePath, EXTENSION fileExtension, std::string data) {
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	this->gamma = gamma;
	this->exposure = exposure;
	this->aspectRatio = aspectRatio;
	this->filePath = filePath;
	this->fileExtension = fileExtension;
	this->data = data;
}

Image::Image(const std::string& _filePath, IO_MODE _mode, int _resolutionWidth, int _resolutionHeight, EXTENSION _extension) :
	filePath(_filePath), mode(_mode), resolutionWidth(_resolutionWidth), resolutionHeight(_resolutionHeight), extension(_extension), bytes_per_pixel(0), data(nullptr) {

	if (this->mode == READ) {
		this->bytes_per_pixel = 0;
	}
	else {
		// this->bytes_per_pixel gets overriden in readImage, but for writing it is not
		if (this->extension == PNG) {
			this->bytes_per_pixel = 3; // RGB
		}
		else if (this->extension == PPM) {
			this->bytes_per_pixel = 3;
		}
		else if (this->extension == JPG) {
			this->bytes_per_pixel = 3;
		}
	}
}

Image::Image(const ImageData& imageData) {
	this->filePath = imageData.filePath;
	this->mode = WRITE; // Assumed as write as this constructor would typically be called in main() function
	this->extension = imageData.fileExtension;
	this->resolutionWidth = imageData.imageWidth;
	this->resolutionHeight = imageData.imageHeight;
	this->strData = imageData.data;
	this->data = this->convertStringToChar(this->strData);
	if (this->mode == READ) {
		this->bytes_per_pixel = 0;
	}
	else {
		// this->bytes_per_pixel gets overriden in readImage, but for writing it is not
		if (this->extension == PNG) {
			this->bytes_per_pixel = 4; // RGB
		}
		else if (this->extension == PPM) {
			this->bytes_per_pixel = 3;
		}
		else if (this->extension == JPG) {
			this->bytes_per_pixel = 3;
		}
	}
}

Image::~Image() {
	
}

std::string Image::readImage() {
	// Get image data before loading

	// #8 Bytes per pixel (4 = RGBA, 3 = RGB, 2 = GA, 1 = G)
	bool isSupported = stbi_info(this->filePath.c_str(), &this->resolutionWidth, &this->resolutionHeight, &this->bytes_per_pixel);
	if (!isSupported) {
		std::cerr << "Error: (Invalid Access Attempt to File): Target file is unsupported" << std::endl;
	}

	// Load Image
	this->data = stbi_load(this->filePath.c_str(), &this->resolutionWidth, &this->resolutionHeight, &this->bytes_per_pixel, this->bytes_per_pixel);

	if (!data) {
		std::cerr << "Error: (Invalid Access Attempt to File): stbi_load() loaded a corrupted or invalid file" << std::endl;
	}

	std::string stringData = this->convertCharToString(this->data);
	stbi_image_free(this->data);
	return stringData;
}

void Image::writeImage() {

	this->data = this->convertStringToChar(this->strData);

	if (this->extension == PNG) {
		if (stbi_write_png(this->filePath.c_str(), this->resolutionWidth, this->resolutionHeight, this->bytes_per_pixel, this->data, this->resolutionWidth * this->bytes_per_pixel) == 0) {
			// Failure (will later implement Error(), for now use std::cerr)
			std::cerr << "Error: (Invalid Access Attempt to File): stbi_write_png() wrote corrupted data" << std::endl;
		}
	}
	else if (this->extension == JPG) {
		if (stbi_write_jpg(this->filePath.c_str(), this->resolutionWidth, this->resolutionHeight, this->bytes_per_pixel, this->data, 100) == 0) {
			// 100% Quality: Why? Because I am 100% SIGMA and you are 100% BETA
			std::cerr << "Error: (Invalid Access Attempt to File): stbi_write_jpg() wrote corruped data" << std::endl;
		}
	}
	else if (this->extension == PPM) {
		// File Format:
		// P6 Width Height 255 \n
		// RGB . . .
		// .
		// .
		// .
		assert(this->bytes_per_pixel == 3);

		std::ofstream outputFile(this->filePath);

		outputFile << "P6 " << this->resolutionWidth << ' ' << this->resolutionHeight << "\n255\n"; // Magic Number
		
		std::cerr << &this->data << std::endl;

		int bytes_per_scanline = this->resolutionWidth * this->bytes_per_pixel;

		for (int i = 0; i < this->resolutionHeight; ++i) {
			std::cerr << "Info: (Image Writing Progress): Scanlines Remaining: " << (this->resolutionHeight - i) << std::flush;
			for (int j = 0; j < bytes_per_scanline; ++j) {
				unsigned char value = static_cast<int>(this->strData[i * bytes_per_scanline + j]);
				outputFile << value << ' ';
			}
			outputFile << "\n";
		}

		outputFile.close();
	}
}

void Image::setData(std::string& strData) {
	this->strData = strData;
}

void Image::setMode(IO_MODE newMode) {
	this->mode = newMode;
}

unsigned char* Image::convertStringToChar(std::string& strData) {
	return reinterpret_cast<unsigned char*>(const_cast<char*>(strData.c_str()));
}

std::string Image::convertCharToString(unsigned char* data) {
	return std::string(reinterpret_cast<char*>(data));
}