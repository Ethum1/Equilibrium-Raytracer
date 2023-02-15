#pragma once

#include <cmath>

#include "Color.h"
#include "Vector.h"
#include "Noise.h"


class Texture {
public:
	virtual Color value(const Vector2f& uv, const Vector3f& p) const = 0;
};

// Perlin Noise
class NoiseTexture : public Texture {
public:
	NoiseTexture(float _scale = 1.0f) {
		this->scale = _scale;
		this->c0 = Color(0.8f, 0.0f, 0.0f);
		this->c1 = Color(0.0f, 0.0f, 0.8f);
	}

	NoiseTexture(const Color& _c0, const Color& _c1, float _scale = 1.0f) : c0(_c0), c1(_c1), scale(_scale) {}

	virtual Color value(const Vector2f& uv, const Vector3f& p) const override;

public:
	float scale;
	Color c0, c1;
	SolidNoise solidNoise;
};

// Constant Color
class SolidTexture : public Texture {
public:
	SolidTexture(Color c) : color(c) {}

	virtual Color value(const Vector2f& uv, const Vector3f& p) const override;

public:
	Color color;
};

class MarbleTexture : public Texture {
public:
	MarbleTexture(float stripesPerUnit, float _scale, int _octaves);

	MarbleTexture(const Color& _c0, const Color& _c1, const Color& _c2, float stripesPerUnit, float _scale, int _octaves);

	virtual Color value(const Vector2f& uv, const Vector3f& p) const override;

public:
	float freq, scale;
	int octaves;
	Color c0, c1, c2;
	SolidNoise noise;
};