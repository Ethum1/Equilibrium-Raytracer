#pragma once

#include <memory>

#include "Vector.h"
#include "Normal.h"

struct Ray {
public:
	Ray() : t(0.0f), ior(1.0f) {}
	Ray(const Point3f& _origin, const Vector3f& _direction, float _time = 0.0f) : o(_origin), t(_time), ior(1.0f) { setDirection(_direction); }
	Ray(const Point3f& _origin, const Vector3f& _direction, float _time = 0.0f, float _ior = 1.0f) : o(_origin), t(_time), ior(_ior) { setDirection(_direction); }

	Point3f origin() const { return this->o; }
	Vector3f direction() const { return this->dir; }
	Vector3f invDirection() const {
		return Vector3f(1.0f / this->dir.x(), 1.0f / this->dir.y(), 1.0f / this->dir.z());
	}
	Vector3i sign() const { return this->signDir; }
	float time() const { return this->t; }

	void setOrigin(Point3f newOrigin) {
		this->o = newOrigin;
	}

	void setDirection(Vector3f newDirection) {
		this->dir = newDirection;
		this->signDir.setX(this->dir.x() > 0 ? 0 : 1);
		this->signDir.setY(this->dir.y() > 0 ? 0 : 1);
		this->signDir.setZ(this->dir.z() > 0 ? 0 : 1);
	}

	Point3f at(float t) const { return this->o + t * this->dir; }

	Point3f operator[](float t) const { return this->o + t * this->dir; }

private:
	Point3f o;
	Vector3f dir;
	Vector3i signDir; // Positive / Negative Sign of Direction Vector
	float t; // Time when the ray is shot (out of my penis, presumably into your mom's pussy)
	float ior; // Index of Refraction of the current medium that the ray is traveling through (is updated at every boundary intersection)
};