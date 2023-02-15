#pragma once

#include "Shape.h"

class Primitive : public Shape {
public:
	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const = 0;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const = 0; // Hit but for shadow rays
};

class Sphere : public Primitive {
public:
	Sphere(const Vector3f& _center, float radius, std::shared_ptr<Material> _mat);
	
	virtual bool hasBoundingBox() const override;
	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override;
	virtual AABB boundingBox(float time0, float time1) const override;

private:
	Vector3f center;
	float radius;
	std::shared_ptr<Material> mat;
};

// Triangle, Rectangle, Box (to be implemented in the future) 

// Essentially a parallelogram but, with 90 degree angles, we assert that this is true
class Rectangle : public Primitive {
public:
	Rectangle(const Point3f& _origin, const Point3f& a, const Point3f& b, std::shared_ptr<Material> _mat);

	virtual bool hasBoundingBox() const override;
	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override;
	virtual AABB boundingBox(float time0, float time1) const override;

private:
	Point3f origin;
	Vector3f u, v;
	ONBf uvw; // U & V are provided in the constructor
	float area; // Magnitude of the Cross Product of U & V
	std::shared_ptr<Material> mat;
};

// Box

class Box : public Primitive {
public:
	Box() {}
	Box(const Point3f& a, const Point3f& b, std::shared_ptr<Material> _mat) : minPoint(a), maxPoint(b), mat(_mat) {}
	
	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;
	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override;
	virtual bool hasBoundingBox() const override;
	virtual AABB boundingBox(float time0, float time1) const override;

private:
	std::shared_ptr<Material> mat;
	Point3f minPoint, maxPoint;
	ONBf uvw;
};

// XY, XZ, and YZ rectangles for debugging purposes

class XYRect : public Primitive {
public:
	XYRect() {}
	XYRect(float _k, float _x0, float _y0, float _x1, float _y1, std::shared_ptr<Material> _mat) : k(_k), x0(_x0), y0(_y0), x1(_x1), y1(_y1), mat(_mat) {}

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;

	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override;

	virtual bool hasBoundingBox() const override;

	virtual AABB boundingBox(float time0, float time1) const override;

private:
	std::shared_ptr<Material> mat;
	float x0, y0, x1, y1, k;
};

class XZRect : public Primitive {
public:
	XZRect() {}
	XZRect(float _k, float _x0, float _z0, float _x1, float _z1, std::shared_ptr<Material> _mat) : k(_k), x0(_x0), z0(_z0), x1(_x1), z1(_z1), mat(_mat) {}

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;

	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override;

	virtual bool hasBoundingBox() const override;

	virtual AABB boundingBox(float time0, float time1) const override;

private:
	std::shared_ptr<Material> mat;
	float x0, z0, x1, z1, k;
};

class YZRect : public Primitive {
public:
	YZRect() {}
	YZRect(float _k, float _y0, float _z0, float _y1, float _z1, std::shared_ptr<Material> _mat) : k(_k), y0(_y0), z0(_z0), y1(_y1), z1(_z1), mat(_mat) {}

	virtual bool hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const override;

	virtual bool shadowHit(const Ray& r, float tmin, float tmax, float time) const override;

	virtual bool hasBoundingBox() const override;

	virtual AABB boundingBox(float time0, float time1) const override;

private:
	std::shared_ptr<Material> mat;
	float y0, z0, y1, z1, k;
};