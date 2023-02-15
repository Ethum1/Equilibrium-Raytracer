#include "Scene.h"

bool Scene::hit(const Ray& r, float tmin, float tmax, float time, Interaction& record) const {
	Interaction temp; 
	bool hitAnything = false;
	float closest = tmax;

	for (const std::shared_ptr<Shape>& object : this->objects) {
		if (object->hit(r, tmin, closest, time, temp)) {
			hitAnything = true;
			closest = temp.t;
			record = temp;
		}
	}

	return hitAnything;
}

AABB Scene::boundingBox(float time0, float time1) const {
	AABB temp;

	if (this->objects[0]->hasBoundingBox()) {
		temp = this->objects[0]->boundingBox(time0, time1);
	}
	
	for (int i = 1; i < this->objects.size(); ++i) {
		if (this->objects[i]->hasBoundingBox()) {
			temp = surround(temp, this->objects[i]->boundingBox(0.0f, 0.0f));
		}
	}

	return temp;
}

bool Scene::shadowHit(const Ray& r, float tmin, float tmax, float time) const {
	bool hitAnything = false;
	float closest = tmax;

	for (const std::shared_ptr<Shape>& object : this->objects) {
		if (object->shadowHit(r, tmin, closest, time)) {
			hitAnything = true;
		}
	}

	return hitAnything;
}