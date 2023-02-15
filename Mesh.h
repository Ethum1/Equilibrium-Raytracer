#pragma once

#include "Shape.h"

class Mesh : public Shape {
public:
	virtual bool hasBoundingBox() const override;
	virtual AABB boundingBox(float time0, float time1) const override;
	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override; // Hit but for shadow rays
};