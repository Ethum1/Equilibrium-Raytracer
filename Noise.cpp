#include "Noise.h"

SolidNoise::SolidNoise() {
	this->grad.reserve(16);
	this->phi.reserve(16);

	this->grad[0] = Vector3f(1, 1, 0);
	this->grad[1] = Vector3f(-1, 1, 0);
	this->grad[2] = Vector3f(1, -1, 0);
	this->grad[3] = Vector3f(-1, -1, 0);

	this->grad[4] = Vector3f(1, 0, 1);
	this->grad[5] = Vector3f(-1, 0, 1);
	this->grad[6] = Vector3f(1, 0, -1);
	this->grad[7] = Vector3f(-1, 0, -1);
	
	this->grad[8] = Vector3f(0, 1, 1);
	this->grad[9] = Vector3f(0, -1, 1);
	this->grad[10] = Vector3f(0, 1, -1);
	this->grad[11] = Vector3f(0, -1, -1);

	this->grad[12] = Vector3f(1, 1, 0);
	this->grad[13] = Vector3f(-1, 1, 0);
	this->grad[14] = Vector3f(0, -1, 1);
	this->grad[15] = Vector3f(0, -1, -1);
	
	for (int i = 0; i < 16; ++i) {
		this->phi[i] = i;
	}

	for (int i = 14; i >= 0; --i) {
		int target = static_cast<int>(EQMath::random(0.0f, 0.999f) * i);
		int temp = this->phi[i + 1];
		this->phi[i + 1] = this->phi[target];
		this->phi[target] = temp;
	}
}

float SolidNoise::turbulence(const Vector3f& p, int depth) const {
	float sum = 0.0f;
	float weight = 1.0f;
	Vector3f ptemp(p);

	sum = std::fabs(noise(ptemp));

	for (int i = 1; i < depth; ++i) {
		weight = weight * 2;
		ptemp.setX(p.x() * weight);
		ptemp.setY(p.y() * weight);
		ptemp.setZ(p.z() * weight);

		sum += std::fabs(noise(ptemp)) / weight;
	}
	
	return sum;
}

float SolidNoise::dturbulence(const Vector3f& p, int depth, float d) const {
	float sum = 0.0f;
	float weight = 1.0f;
	Vector3f ptemp(p);

	sum += std::fabs(noise(ptemp)) / d;

	for (int i = 1; i < depth; ++i) {
		weight = weight * d;
		ptemp.setX(p.x() * weight);
		ptemp.setY(p.y() * weight);
		ptemp.setZ(p.z() * weight);

		sum += std::fabs(noise(ptemp)) / d;
	}

	return sum;
}

float SolidNoise::noise(const Vector3f& p) const {
	int fi, fj, fk;
	float fu, fv, fw, sum;
	Vector3f v;

	fi = static_cast<int>(std::floor(p.x()));
	fj = static_cast<int>(std::floor(p.y()));
	fk = static_cast<int>(std::floor(p.z()));

	fu = p.x() - static_cast<float>(fi);
	fv = p.y() - static_cast<float>(fj);
	fw = p.z() - static_cast<float>(fk);
	
	sum = 0.0f;

	v = Vector3f(fu, fv, fw);
	sum += knot(fi, fj, fk, v);

	v = Vector3f(fu - 1, fv, fw);
	sum += knot(fi + 1, fj, fk, v);

	v = Vector3f(fu, fv - 1, fw);
	sum += knot(fi, fj + 1, fk, v);

	v = Vector3f(fu, fv, fw - 1);
	sum += knot(fi, fj, fk + 1, v);

	v = Vector3f(fu - 1, fv - 1, fw);
	sum += knot(fi + 1, fj + 1, fk, v);

	v = Vector3f(fu - 1, fv, fw - 1);
	sum += knot(fi + 1, fj, fk + 1, v);

	v = Vector3f(fu, fv - 1, fw - 1);
	sum += knot(fi, fj + 1, fk + 1, v);

	v = Vector3f(fu - 1, fv - 1, fw - 1);
	sum += knot(fi + 1, fj + 1, fk + 1, v);

	return sum;
}