#pragma once

#include <vector>
#include <memory>

#include "Shape.h"
#include "AABB.h"
#include "BVHSplit.h"

class BVH_Node : public Shape {
public:
	BVH_Node();
	BVH_Node(const std::vector<std::shared_ptr<Shape>>& shapes, int numShapes);
	BVH_Node(std::shared_ptr<Shape> a, std::shared_ptr<Shape> b);
	BVH_Node(std::shared_ptr<Shape> a, std::shared_ptr<Shape> b, const AABB& _bbox);

	bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& rec) const;
	bool shadowHit(const Ray& r, float tmin, float tmax, float time) const;
	bool hasBoundingBox() const { return true; }
	AABB boundingBox(float time0, float time1) const;
	std::shared_ptr<Shape> buildBranch(std::vector<std::shared_ptr<Shape>> shapes, int numShapes, int axis = 0);
public:
	AABB bbox;
	std::shared_ptr<Shape> left;
	std::shared_ptr<Shape> right;
};