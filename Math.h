#pragma once

#include <cmath>
#include <limits>
#include <random>

namespace EQMath {
	// Useful Constants
	const float PI = 3.1415927f;
	const float INVPI = 0.31830987f;
	const float INF = std::numeric_limits<float>::infinity();

	// Utility Functions
	
	// Man Knows Trigonometry. . .
	inline float degtorad(float deg) {
		return deg * PI / 180.0f;
	}

	inline float radtodeg(float rad) {
		return rad * 180.0f / PI;
	}

	inline float clamp(float t, float min = 0.0f, float max = 1.0f) {
		if (t < min) {
			return min;
		}
		else if (t > max) {
			return max;
		}
		else {
			return t;
		}
	}

	// Random Number Generation
	inline float random(float min, float max) {
		static std::uniform_real_distribution<float> distribution(min, max);
		static std::default_random_engine generator;
		return distribution(generator);
	}

	inline Vector2f generateRandomVector2(float min, float max) {
		return Vector2f(random(min, max), random(min, max));
	}

	inline Vector3f generateRandomVector3(float min, float max) {
		return Vector3f(random(min, max), random(min, max), random(min, max));
	}

	inline Vector3f randomInUnitSphere() {
		do {
			Vector3f p = generateRandomVector3(-1.0f, 1.0f);
			if (p.squaredMagnitude() >= 1.0f) {
				continue;
			}
			return p;
		} while (true);
	}

	inline Vector3f randomInHemisphere(const Normal3f& normal) {
		Vector3f randomVector = randomInUnitSphere();
		if (dot(randomVector, normal) > 0.0f) { // Same Hemisphere as Normal
			return randomVector;
		}
		else {
			return -randomVector;
		}
	}

	inline Vector2f randomInUnitDisk() {
		do {
			Vector2f p = generateRandomVector2(-1.0f, 1.0f);
			if (p.squaredMagnitude() >= 1) {
				continue;
			}
			return p;
		} while (true);
	}

	inline Vector3f randomUnitVector() {
		return normalize(randomInUnitSphere());
	}

	// When we use spherical coordinates, we assume normalized vectors (Theta, Phi)
	inline Vector2f xyzToSphere(const Vector3f& omega) {
		return Vector2f(std::acos(omega.y()), std::atan(omega.z() / omega.x()));
	}

	inline Vector3f sphereToXYZ(const Vector2f& omega) {
		return Vector3f(
			std::sin(omega.x()) * std::cos(omega.y()),
			std::cos(omega.x()),
			std::sin(omega.x()) * std::sin(omega.y())
		);
	}
}