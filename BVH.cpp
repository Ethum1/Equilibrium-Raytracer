#include "BVH.h"

BVH_Node::BVH_Node() {}

BVH_Node::BVH_Node(std::shared_ptr<Shape> a, std::shared_ptr<Shape> b, const AABB& _bbox) {
	this->bbox = _bbox;
	this->left = a;
	this->right = b;
}

BVH_Node::BVH_Node(std::shared_ptr<Shape> a, std::shared_ptr<Shape> b) {
	this->left = a;
	this->right = b;
	this->bbox = surround(left->boundingBox(0.0f, 0.0f), right->boundingBox(0.0f, 0.0f));
}

BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Shape>>& shapes, int numShapes) {
	// If the number of shapes is 1 or 2, then there are no branches in the tree, just leafs
	if (numShapes == 1) {
		*this = BVH_Node(shapes[0], shapes[0]);
	}
	else if (numShapes == 2) {
		*this = BVH_Node(shapes[0], shapes[1]);
	}
	else {
		this->bbox = shapes[0]->boundingBox(0.0f, 0.0f);
		for (int i = 1; i < numShapes; ++i) {
			this->bbox = surround(this->bbox, shapes[i]->boundingBox(0.0f, 0.0f));
		}

		
		Vector3f pivot = (this->bbox.max() + this->bbox.min()) / 2.0f;

		int midpoint = medianSplit(shapes, numShapes, pivot.x(), 0);

		this->left = buildBranch(shapes, midpoint, 1);

		this->right = buildBranch(std::vector<std::shared_ptr<Shape>>(shapes.begin() + midpoint, shapes.end()), numShapes - midpoint, 1);
	}
}

AABB BVH_Node::boundingBox(float time0, float time1) const {
	return this->bbox;
}

bool BVH_Node::hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const {
	if (this->hasBoundingBox()) {
		if (!(this->bbox.rayIntersect(r, tmin, tmax))) {
			return false;
		}
	}
	else {
		// For primitives such as triangles, rectangles, and planes which cannot be bounded
		if (!(this->hit(r, tmin, tmax, time, record))) {
			return false;
		}
	}

	bool hitsLeft = false;
	bool hitsRight = false;
	record.t = tmax;

	hitsLeft = this->left->hit(r, tmin, tmax, time, record);
	hitsRight = this->right->hit(r, tmin, hitsLeft ? record.t : tmax, time, record);

	return (hitsLeft || hitsRight);
}

bool BVH_Node::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	if (!(this->bbox.rayIntersect(r, tmin, tmax))) {
		return false;
	}

	if (this->right->shadowHit(r, tmin, tmax, time)) {
		return true;
	}
	
	return this->left->shadowHit(r, tmin, tmax, time);
}

std::shared_ptr<Shape> BVH_Node::buildBranch(std::vector<std::shared_ptr<Shape>> shapes, int numShapes, int axis) {
	// If the number of shapes is 1, then the sub-branch is just a leaf node, in which case we return just the shape itself
	// If the number of shapes is 2, then the sub-branch continues until there are no more shapes so we return a BVH_Node
	// Remember that all primitives and all meshes are shapes (subclasses) so we return shape as an umbrella term
	if (numShapes == 1) {
		return shapes[0];
	}
	else if (numShapes == 2) {
		return std::make_shared<BVH_Node>(BVH_Node(shapes[0], shapes[1]));
	}
	else {
		AABB box = shapes[0]->boundingBox(0.0f, 0.0f);
		for (int i = 1; i < numShapes; ++i) {
			box = surround(box, shapes[i]->boundingBox(0.0f, 0.0f));
		}

		Vector3f pivot = (box.max() + box.min()) / 2.0f;

		int midpoint = -1;

		if (axis == 0) {
			midpoint = medianSplit(shapes, numShapes, pivot.x(), axis);
		}
		else if (axis == 1) {
			midpoint = medianSplit(shapes, numShapes, pivot.y(), axis);
		}
		else {
			midpoint = medianSplit(shapes, numShapes, pivot.z(), axis);
		}

		std::shared_ptr<Shape> newLeft = buildBranch(shapes, midpoint, (axis + 1) % 3);
		std::shared_ptr<Shape> newRight = buildBranch(std::vector<std::shared_ptr<Shape>>(shapes.begin() + midpoint, shapes.end()), numShapes - midpoint, (axis + 1) % 3);

		return std::make_shared<BVH_Node>(newLeft, newRight, box);
	}
}