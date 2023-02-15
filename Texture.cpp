#include "Texture.h"

Color NoiseTexture::value(const Vector2f& uv, const Vector3f& p) const {
	float t = (1.0f + this->solidNoise.noise(p * this->scale)) / 2.0f;
	return t * c0 + (1.0f - t) * c1;
}

Color SolidTexture::value(const Vector2f& uv, const Vector3f& p) const {
	return this->color;
}

MarbleTexture::MarbleTexture(float stripesPerUnit, float _scale = 5.0f, int _octaves = 8) {
	this->freq = EQMath::PI * stripesPerUnit;
	this->scale = _scale;
	this->octaves = _octaves;
	this->c0 = Color(0.8f, 0.8f, 0.8f);
	this->c1 = Color(0.4f, 0.2f, 0.1f);
	this->c2 = Color(0.06f, 0.04f, 0.02f);
}

MarbleTexture::MarbleTexture(const Color& _c0, const Color& _c1, const Color& _c2, float stripesPerUnit, float _scale = 3.0f, int _octaves = 8) {
	this->c0 = _c0;
	this->c1 = _c1;
	this->c2 = _c2;
	this->freq = EQMath::PI * stripesPerUnit;
	this->scale = _scale;
	this->octaves = _octaves;
}

Color MarbleTexture::value(const Vector2f& uv, const Vector3f& p) const {
	float temp = this->scale * this->noise.turbulence(this->freq * p, this->octaves);
	float t = 2.0f * std::fabs(std::sin(this->freq * p.x() + temp));

	if (t < 1.0f) {
		return (this->c1 * t + (1.0f - t) * c2);
	}
	else {
		t -= 1.0f;
		return (c0 * t + (1.0f - t) * c1);
	}
}