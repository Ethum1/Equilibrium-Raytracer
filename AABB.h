#pragma once

#include <vector>
#include <memory>

#include "Ray.h"


// Axis Aligned Bounding Box (AABB): This is different from Box, which is a shape.
class AABB {
public:
	AABB() {}
	AABB(const Point3f& a, const Point3f& b) { this->points[0] = a; this->points[1] = b; }
	Point3f min() const { return this->points[0]; }
	Point3f max() const { return this->points[1]; }

	bool rayIntersect(const Ray& r, float tmin, float tmax) const;

	friend AABB surround(const AABB& box1, const AABB& box2);
public:
	Point3f points[2]; // Size 2
};