#pragma once

#include <cmath>
#include <vector>

#include "Vector.h"
#include "Math.h"

class SolidNoise {
public:
	SolidNoise();
	float noise(const Vector3f& p) const;
	float turbulence(const Vector3f& p, int depth) const;
	float dturbulence(const Vector3f& p, int depth, float d) const;
	float omega(float t) const;
	Vector3f gamma(int i, int j, int k) const;
	int intGamma(int i, int j) const;
	float knot(int i, int j, int k, Vector3f& v) const;

	std::vector<Vector3f> grad; // Size 16
	std::vector<int> phi; // Size 16
};

inline float SolidNoise::omega(float t) const {
	t = (t > 0.0f) ? t : -t;
	
	return (-6.0f * t * t * t * t * t + 15.0f * t * t * t * t - 10.0f * t * t * t + 1.0f);
}

inline Vector3f SolidNoise::gamma(int i, int j, int k) const {
	int idx;
	idx = this->phi[std::abs(k) % 16];
	idx = this->phi[std::abs(j + idx) % 16];
	idx = this->phi[std::abs(i + idx) % 16];
	return this->grad[idx];
}

inline float SolidNoise::knot(int i, int j, int k, Vector3f& v) const {
	return (omega(v.x()) * omega(v.y()) * omega(v.z()) * dot(gamma(i, j, k), v));
}

inline int SolidNoise::intGamma(int i, int j) const {
	int idx;
	idx = this->phi[std::abs(j) % 16];
	idx = this->phi[std::abs(i + idx) % 16];
	return idx;
}