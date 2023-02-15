#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "Shape.h"

class Scene : public Shape {
public:
	Scene() {};
	Scene(std::shared_ptr<Shape> object) { add(object); }

	void clear() { this->objects.clear(); }
	void add(std::shared_ptr<Shape> object) { this->objects.push_back(object); }

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override; // Hit but for shadow rays
	virtual bool hasBoundingBox() const override { return true; }
	virtual AABB boundingBox(float time0, float time1) const override;
	
public:
	std::vector<std::shared_ptr<Shape>> objects;
};