#include "BVHSplit.h"

int medianSplit(std::vector<std::shared_ptr<Shape>> list, int size, float pivot, int axis) {
	AABB bbox;
	float centroid;
	int res = 0;

	for (int i = 0; i < size; ++i) {
		bbox = list[i]->boundingBox(0.0f, 0.0f);
		if (axis == 0) {
			centroid = ((bbox.min().x()) + (bbox.max().x())) / 2.0f;
		}
		else if (axis == 1) {
			centroid = ((bbox.min().y()) + (bbox.max().y())) / 2.0f;
		}
		else {
			centroid = ((bbox.min().z()) + (bbox.max().z())) / 2.0f;
		}

		if (centroid < pivot) {
			std::shared_ptr<Shape> temp = list[i];
			list[i] = list[res];
			list[res] = temp;
			++res;
		}
	}

	if (res == 0 || res == size) {
		res = size / 2;
	}

	return res;
}