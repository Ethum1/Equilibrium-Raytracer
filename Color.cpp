#include "Color.h"

Color& Color::operator+=(const Color& c) {
	this->r += c.r;
	this->g += c.g;
	this->b += c.b;
	return *this;
}

Color operator+(const Color& a, const Color& b) {
	return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}

Color& Color::operator-=(const Color& c) {
	this->r -= c.r;
	this->g -= c.g;
	this->b -= c.b;
	return *this;
}

Color operator-(const Color& a, const Color& b) {
	return Color(a.r - b.r, a.g - b.g, a.b - b.b);
}

Color& Color::operator*=(float s) {
	this->r *= s;
	this->g *= s;
	this->b *= s;
	return *this;
}

Color operator*(const Color& a, float s) {
	return Color(a.r * s, a.g * s, a.b * s);
}

Color operator*(float s, const Color& b) {
	return Color(b.r * s, b.g * s, b.b * s);
}

Color operator*(const Color& a, const Color& b) {
	return Color(a.r * b.r, a.g * b.g, a.b * b.b);
}

Color& Color::operator/=(float s) {
	return *this *= (1 / s);
}

Color operator/(const Color& a, float s) {
	return Color(a.r * (1 / s), a.g * (1 / s), a.b * (1 / s));
}

bool operator==(const Color& a, const Color& b) {
	return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

bool operator!=(const Color& a, const Color& b) {
	return (a.r != b.r) && (a.g != b.g) && (a.b != b.b);
}

void Color::clamp(float min, float max) {
	// Clamp R
	if (this->r < min) 
		this->r = min;
	if (this->r > max) 
		this->r = max;
	// Clamp G
	if (this->g < min) 
		this->g = min;
	if (this->g > max) 
		this->g = max;
	// Clamp B
	if (this->b < min) 
		this->b = min;
	if (this->b > max) 
		this->b = max;
}

// Utility Functions

void gammaCorrect(Color& color, float gamma) {
	color.r = std::pow(color.r, (1.0f / gamma));
	color.g = std::pow(color.g, (1.0f / gamma));
	color.g = std::pow(color.b, (1.0f / gamma));
}

Color gammaCorrect(const Color& color, float gamma) {
	return Color(
		std::pow(color.r, (1.0f / gamma)),
		std::pow(color.g, (1.0f / gamma)),
		std::pow(color.b, (1.0f / gamma))
	);
}

// Assume that the value is clamped
Vector3i colorToInt(Color color) {
	return Vector3i(
		static_cast<int>(256.0f * color.r),
		static_cast<int>(256.0f * color.g),
		static_cast<int>(256.0f * color.b)
	);
}

void writeColorToFile(std::string& fileData, const Color& pixelColor, int samples, float gamma) {
	float R = pixelColor.r;
	float G = pixelColor.g;
	float B = pixelColor.b;

	R = std::pow(R / samples, (1.0f / gamma));
	G = std::pow(G / samples, (1.0f / gamma));
	B = std::pow(B / samples, (1.0f / gamma));

	fileData += static_cast<int>(256.0f * EQMath::clamp(R, 0.0f, 0.999f));
	fileData += static_cast<int>(256.0f * EQMath::clamp(G, 0.0f, 0.999f));
	fileData += static_cast<int>(256.0f * EQMath::clamp(B, 0.0f, 0.999f));
}