#include "Primitive.h"

Sphere::Sphere(const Vector3f& _center, float _radius, std::shared_ptr<Material> _mat) {
	this->center = _center;
	this->radius = _radius;
	this->mat = _mat;
	
}

bool Sphere::hit(const Ray& r, float tmin, float tmax, float time, Interaction& rec) const {
	Point3f temp = r.origin() - this->center;
	float a = r.direction().squaredMagnitude();
	float halfB = dot(temp, r.direction());
	float c = temp.squaredMagnitude() - (this->radius * this->radius);

	float discriminant = halfB * halfB - a * c;
	
	// wow the solution to this quadratic is UNREAL!!! LMFAO
	if (discriminant < 0) {
		return false;
	}

	float sqrtd = std::sqrt(discriminant);

	float root = (-halfB - sqrtd) / a;
	if (root < tmin || tmax < root) {
		root = (-halfB + sqrtd) / a;
		if (root < tmin || tmax < root) {
			return false;
		}
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	rec.setNormal(r, normalize((rec.p - this->center) / this->radius));
	rec.material = this->mat;
	// UV Coordinates will be implemented later (for now set any arbitrary value)
	rec.uv = Vector2f(0.0f, 0.0f);

	return true;
}

bool Sphere::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	Vector3f temp = r.origin() - this->center;
	float a = r.direction().squaredMagnitude();
	float halfB = dot(temp, r.direction());
	float c = temp.squaredMagnitude() - (this->radius * this->radius);

	float discriminant = (halfB * halfB) - (a * c);

	// wow the solution to this quadratic is UNREAL!!! LMFAO
	if (discriminant < 0) {
		return false;
	}

	float sqrtd = std::sqrt(discriminant);

	float root = (-halfB - sqrtd) / a;
	if (root < tmin || tmax < root) {
		root = (-halfB + sqrtd) / a;
		if (root < tmin || tmax < root) {
			return false;
		}
	}

	return true;
}

AABB Sphere::boundingBox(float time0, float time1) const {
	return AABB(
		this->center - Vector3f(this->radius, this->radius, this->radius),
		this->center + Vector3f(this->radius, this->radius, this->radius)
	);
}

bool Sphere::hasBoundingBox() const {
	return true;
}

// Rectangle



Rectangle::Rectangle(const Point3f& _origin, const Point3f& a, const Point3f& b, std::shared_ptr<Material> _mat) : origin(_origin), mat(_mat) {
	this->u = Vector3f(a - _origin);
	this->v = Vector3f(b - _origin);
	this->uvw.buildFromUV(this->u, this->v);

	std::cerr << "Normal: " << this->uvw.W().x() << " " << this->uvw.W().y() << " " << this->uvw.W().z() << " Material Type: " << this->mat->getType() << std::endl;
	// this->area = this->u.magnitude() * this->v.magnitude();
}

bool Rectangle::hasBoundingBox() const {
	return true;
}

bool Rectangle::hit(const Ray& r, float tmin, float tmax, float time, Interaction& rec) const {
	float angleOfIntersection = dot(r.direction(), this->uvw.W());

	

	// If the angle is 0, that means it is parallel
	if (angleOfIntersection < 0.0001f && angleOfIntersection > -0.0001f) {
		return false;
	}

	//float t = (dot(this->uvw.W(), this->origin) - dot(this->uvw.W(), r.origin())) / (angleOfIntersection);
	float t = dot((this->origin - r.origin()), this->uvw.W()) / dot(r.direction(), this->uvw.W());

	if (t > tmax || t < tmin) {
		return false;
	}

	Vector3f hitLocation(r.origin() + r.direction() * t);
	Vector3f offset(hitLocation - this->origin);

	float u = dot(this->uvw.U(), offset) / this->u.magnitude();
	if (u < 0.0f || u > 1.0f) {
		return false;
	}
	float v = dot(this->uvw.V(), offset) / this->v.magnitude();
	if (v < 0.0f || v > 1.0f) {
		return false;
	}

	rec.material = this->mat;
	rec.p = hitLocation;
	rec.t = t;
	
	
	if (angleOfIntersection < 0.0f) {
		rec.setNormal(r, -this->uvw.W());
	}
	else {
		rec.setNormal(r, this->uvw.W());
	}
	
	rec.uv = Vector2f(0.0f, 0.0f);

	return true;
}

bool Rectangle::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	float angleOfIntersection = dot(r.direction(), this->uvw.W());

	if (angleOfIntersection < 0.0001f && angleOfIntersection > -0.0001f) {
		return false;
	}

	float dot2 = dot(this->uvw.W(), this->origin);
	float t = (dot2 - dot(this->origin, r.origin())) / angleOfIntersection;

	if (t > tmax || t < tmin) {
		return false;
	}

	Vector3f hitLocation(r.origin() + r.direction() * t);
	Vector3f offset(hitLocation - this->origin);

	float u = dot(this->uvw.U(), offset) / this->u.magnitude();
	if (u < 0.0f || u > 1.0f) {
		return false;
	}
	float v = dot(this->uvw.V(), offset) / this->v.magnitude();
	if (v < 0.0f || v > 1.0f) {
		return false;
	}

	return true;
}

AABB Rectangle::boundingBox(float time0, float time1) const {
	Vector3f min, max;

	Vector3f p0 = this->origin;
	Vector3f p1 = this->origin + this->u;
	Vector3f p2 = this->origin + this->v;
	Vector3f p3 = this->origin + this->u + this->v;

	min.setX(p0.x() < p1.x() ? p0.x() : p1.x());
	min.setY(p0.y() < p1.y() ? p0.y() : p1.y());
	min.setZ(p0.z() < p0.z() ? p0.z() : p0.z());

	min.setX(p2.x() < min.x() ? p2.x() : min.x());
	min.setY(p2.y() < min.y() ? p2.y() : min.y());
	min.setZ(p2.z() < min.z() ? p2.z() : min.z());

	min.setX(p3.x() < min.x() ? p3.x() : min.x());
	min.setY(p3.y() < min.y() ? p3.y() : min.y());
	min.setZ(p3.z() < min.z() ? p3.z() : min.z());

	max.setX(p0.x() > p1.x() ? p0.x() : p1.x());
	max.setY(p0.y() > p1.y() ? p0.y() : p1.y());
	max.setZ(p0.z() > p0.z() ? p0.z() : p0.z());

	max.setX(p2.x() > max.x() ? p2.x() : max.x());
	max.setY(p2.y() > max.y() ? p2.y() : max.y());
	max.setZ(p2.z() > max.z() ? p2.z() : max.z());

	max.setX(p3.x() > max.x() ? p3.x() : max.x());
	max.setY(p3.y() > max.y() ? p3.y() : max.y());
	max.setZ(p3.z() > max.z() ? p3.z() : max.z());

	min.setX(min.x() - 0.0001f);
	min.setY(min.y() - 0.0001f);
	min.setZ(min.z() - 0.0001f);

	max.setX(max.x() + 0.0001f);
	max.setY(max.y() + 0.0001f);
	max.setZ(max.z() + 0.0001f);

	return AABB(min, max);
}

// Fucking XY, XZ, YZ rectangles uhh (this is only for debugging purposes)

// XY Rects

bool XYRect::hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const {
	float t = (this->k - r.origin().z()) / r.direction().z();

	if (t < tmin || t > tmax) {
		return false;
	}

	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();

	if (x < this->x0 || x > this->x1 || y < this->y0 || y > this->y1) {
		return false;
	}

	record.uv = Vector2f((x - this->x0) / (this->x1 - this->x0), (y - this->y0) / (this->y1 - this->y0));
	record.t = t;
	record.setNormal(r, Vector3f(0.0f, 0.0f, 1.0f));
	record.material = this->mat;
	record.p = r[t];

	return true;
}

bool XYRect::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	float t = (this->k - r.origin().z()) / r.direction().z();

	if (t < tmin || t > tmax) {
		return false;
	}

	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();

	if (x < this->x0 || x > this->x1 || y < this->y0 || y > this->y1) {
		return false;
	}

	return true;
}

bool XYRect::hasBoundingBox() const {
	return false;
}

AABB XYRect::boundingBox(float time0, float time1) const {
	return AABB(Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f, 0.0f, 0.0f));
}

// XZ Rects

bool XZRect::hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const {
	float t = (this->k - r.origin().y()) / r.direction().y();

	if (t < tmin || t > tmax) {
		return false;
	}

	float x = r.origin().x() + t * r.direction().x();
	float z = r.origin().z() + t * r.direction().z();

	if (x < this->x0 || x > this->x1 || z < this->z0 || z > this->z1) {
		return false;
	}

	record.uv = Vector2f((x - this->x0) / (this->x1 - this->x0), (z - this->z0) / (this->z1 - this->z0));
	record.t = t;
	record.setNormal(r, Vector3f(0.0f, -1.0f, 0.0f));
	record.material = this->mat;
	record.p = r[t];

	return true;
}

bool XZRect::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	float t = (this->k - r.origin().y()) / r.direction().y();

	if (t < tmin || t > tmax) {
		return false;
	}

	float x = r.origin().x() + t * r.direction().x();
	float z = r.origin().z() + t * r.direction().z();

	if (x < this->x0 || x > this->x1 || z < this->z0 || z > this->z1) {
		return false;
	}

	return true;
}

bool XZRect::hasBoundingBox() const {
	return false;
}

AABB XZRect::boundingBox(float time0, float time1) const {
	return AABB(Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f, 0.0f, 0.0f));
}

// YZ Rects

bool YZRect::hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const {
	float t = (this->k - r.origin().x()) / r.direction().x();

	if (t < tmin || t > tmax) {
		return false;
	}

	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();

	if (y < this->y0 || y > this->y1 || z < this->z0 || z > this->z1) {
		return false;
	}

	record.uv = Vector2f((y - this->y0) / (this->y1 - this->y0), (z - this->z0) / (this->z1 - this->z0));
	record.t = t;
	record.setNormal(r, Vector3f(1.0f, 0.0f, 0.0f));
	record.material = this->mat;
	record.p = r[t];

	return true;
}

bool YZRect::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	float t = (this->k - r.origin().x()) / r.direction().x();

	if (t < tmin || t > tmax) {
		return false;
	}

	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();

	if (y < this->y0 || y > this->y1 || z < this->z0 || z > this->z1) {
		return false;
	}

	return true;
}

bool YZRect::hasBoundingBox() const {
	return false;
}

AABB YZRect::boundingBox(float time0, float time1) const {
	return AABB(Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f, 0.0f, 0.0f));
}

// Box

bool Box::hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const {
	// Since the box is the same as the AABB, call the AABB hit function, if yes then write the record
	const AABB& box = this->boundingBox(time, time);

	if (box.rayIntersect(r, tmin, tmax)) {
		record.uv = EQMath::generateRandomVector2(0.0f, 1.0f);
		return true;
	}
	else {
		return false;
	}
}

bool Box::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	return true;
}

bool Box::hasBoundingBox() const {
	return true;
}


AABB Box::boundingBox(float time0, float time1) const {
	return AABB(minPoint, maxPoint);
}