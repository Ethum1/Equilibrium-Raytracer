#include "AABB.h"

AABB surround(const AABB& box1, const AABB& box2) {
	return AABB(
		Point3f(
			box1.min().x() < box2.min().x() ? box1.min().x() : box2.min().x(),
			box1.min().y() < box2.min().y() ? box1.min().y() : box2.min().y(),
			box1.min().z() < box2.min().z() ? box1.min().z() : box2.min().z()
		),
		Point3f(
			box1.max().x() > box2.max().x() ? box1.max().x() : box2.max().x(),
			box1.max().y() > box2.max().y() ? box1.max().y() : box2.max().y(),
			box1.max().z() > box2.max().z() ? box1.max().z() : box2.max().z()
		)
	);
}

bool AABB::rayIntersect(const Ray& r, float tmin, float tmax) const {
	// Using the slab method (Box is defined by 6 axis-aligned lines)
	// 1: Does the ray pass through interval x0 and x1
	// 2: Does the ray pass through interval y0 and y1
	// 3: Does the ray pass through interval z0 and z1
	// If all three are passed then the ray passes through the box

	float intervalMin = tmin; // Current min/max
	float intervalMax = tmax; 

	int sign = r.sign().x();
	float t0 = (this->points[sign].x() - r.origin().x()) * r.invDirection().x();
	float t1 = (this->points[1 - sign].x() - r.origin().x()) * r.invDirection().x();
	if (t0 > intervalMin) {
		intervalMin = t0;
	}
	if (t1 < intervalMax) {
		intervalMax = t1;
	}
	if (intervalMin > intervalMax) {
		return false;
	}

	sign = r.sign().y();
	t0 = (this->points[sign].y() - r.origin().y()) * r.invDirection().y();
	t1 = (this->points[1 - sign].y() - r.origin().y()) * r.invDirection().y();
	if (t0 > intervalMin) {
		intervalMin = t0;
	}
	if (t1 < intervalMax) {
		intervalMax = t1;
	}
	if (intervalMin > intervalMax) {
		return false;
	}

	sign = r.sign().z();
	t0 = (this->points[sign].z() - r.origin().z()) * r.invDirection().z();
	t1 = (this->points[1 - sign].z() - r.origin().z()) * r.invDirection().z();
	if (t0 > intervalMin) {
		intervalMin = t0;
	}
	if (t1 < intervalMax) {
		intervalMax = t1;
	}
	if (intervalMin > intervalMax) {
		return false;
	}
	
	return true;
}