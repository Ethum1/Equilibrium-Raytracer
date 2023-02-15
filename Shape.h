#pragma once

#include "AABB.h"
#include "Material.h"

class Shape {
public:
	virtual bool hasBoundingBox() const = 0;
	virtual AABB boundingBox(float time0, float time1) const = 0;
	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const = 0;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const = 0; // Hit but for shadow rays
};