#pragma once

#include "Vector.h"
#include <cmath>
#include <string>
#include "Math.h"

// Color RGB is additive, operations can be performed on it as if it was a vector
// Color RGB does all of the operations using float (not int) for better lossless quality, at display time it calls the utility function to convert to int

class Color {
public:
	Color() : r(0.0f), g(0.0f), b(0.0f) {}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	Color(float rgb) : r(rgb), g(rgb), b(rgb) {}

	// Add Color
	Color& operator+=(const Color& c);
	friend Color operator+(const Color& a, const Color& b);

	// Subtract Color
	Color& operator-=(const Color& c);
	friend Color operator-(const Color& a, const Color& b);

	// Multiply Color (no dot or cross product, just scalar)
	Color& operator*=(float s);
	friend Color operator*(const Color& a, float s);
	friend Color operator*(float s, const Color& b);
	friend Color operator*(const Color& a, const Color& b);

	// Divide Color
	Color& operator/=(float s);
	friend Color operator/(const Color& a, float s);

	// Compare
	friend bool operator==(const Color& a, const Color& b);
	friend bool operator!=(const Color& a, const Color& b);

	// Clamp (default [0,1])
	void clamp(float min = 0.0f, float max = 1.0f);
public:
	float r, g, b;
};

// Utility Functions

 void gammaCorrect(Color& color, float gamma);

 Color gammaCorrect(const Color& color, float gamma);

 Vector3i colorToInt(Color color);

 void writeColorToFile(std::string& fileData, const Color& pixelColor, int samples, float gamma);