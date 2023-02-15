#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>

template <typename T>
struct Vector2 {
public:
	Vector2() {
		this->e[0] = 0;
		this->e[1] = 0;
	}
	Vector2(T x, T y) {
		this->e[0] = x;
		this->e[1] = y;
	}

	// Vector Operations

	T x() const { return this->e[0]; }
	T y() const { return this->e[1]; }

	void setX(T newX) { this->e[0] = newX; }
	void setY(T newY) { this->e[1] = newY; }

	// Index
	T operator[](int i) const {
		assert((i < 2) && (i >= 0));
		return this->e[i];
	}
	T& operator[](int i) {
		assert((i < 2) && (i >= 0));
		return this->e[i];
	}

	// Add
	Vector2& operator+=(const Vector2& u) {
		this->e[0] += u.x();
		this->e[1] += u.y();
		return *this;
	}
	friend Vector2 operator+(const Vector2& u, const Vector2& v) {
		return Vector2(u.x() + v.x(), u.y() + v.y());
	}

	// Subtract
	Vector2& operator-=(const Vector2& u) {
		this->e[0] -= u.x();
		this->e[1] -= u.y();
		return *this;
	}
	friend Vector2 operator-(const Vector2& u, const Vector2& v) {
		return Vector2(u.x() - v.x(), u.y() - v.y());
	}

	// Multiply
	Vector2& operator*=(const T t) {
		this->e[0] *= t;
		this->e[1] *= t;
		return *this;
	}
	friend Vector2 operator*(const Vector2& u, const T t) {
		return Vector2(u.x() * t, u.y() * t);
	}
	friend Vector2 operator*(const T t, const Vector2& v) {
		return Vector2(v.x() * t, v.y() * t);
	}
	friend Vector2 operator*(const Vector2& u, const Vector2& v) {
		return Vector2(u.x() * v.x(), u.y() * v.y());
	}

	// Divide
	Vector2& operator/=(const T t) {
		return *this *= (1 / t);
	}
	friend Vector2 operator/(const Vector2& u, const T t) {
		return Vector2(u.x() * (1 / t), u.y() * (1 / t));
	}

	// Dot (no cross because this is 2d)
	friend T dot(const Vector2& u, const Vector2& v) {
		return u.x() * v.x() + u.y() * v.y();
	}

	// Inverse
	Vector2 operator-() const {
		return Vector2(-this->e[0], -this->e[1]);
	}

	// Compare
	friend bool operator==(const Vector2& u, const Vector2& v) {
		return (u.x() == v.x()) && (u.y() == v.y());
	}
	friend bool operator!=(const Vector2& u, const Vector2& v) {
		return (u.x() != v.x()) || (u.y() != v.y());
	}

	// Set
	Vector2& operator=(const Vector2& v) {
		this->e[0] = v.x();
		this->e[1] = v.y();
		return *this;
	}

	// Normalize
	friend Vector2 normalize(const Vector2& u) {
		return u / u.magnitude();
	}

	// Length
	float magnitude() const {
		return std::sqrt(squaredMagnitude());
	}

	float squaredMagnitude() const {
		return this->e[0] * this->e[0] + this->e[1] * this->e[1];
	}

	bool nearZero() const {
		const float s = 1e-8;
		return (std::fabs(this->e[0]) < s) && (std::fabs(this->e[1]) < s);
	}

private:
	T e[2];
};

template <typename T>
struct Vector3 {
public:
	Vector3() {
		this->e[0] = 0;
		this->e[1] = 0;
		this->e[2] = 0;
	}
	Vector3(T x, T y, T z) {
		this->e[0] = x;
		this->e[1] = y;
		this->e[2] = z;
	}

	// Vector Operations

	T x() const { return this->e[0]; }
	T y() const { return this->e[1]; }
	T z() const { return this->e[2]; }

	// For Color

	T r() const { return this->e[0]; }
	T g() const { return this->e[1]; }
	T b() const { return this->e[2]; }

	void setX(T newX) { this->e[0] = newX; }
	void setY(T newY) { this->e[1] = newY; }
	void setZ(T newZ) { this->e[2] = newZ; }

	// Index
	T operator[](int i) const {
		assert(i >= 0 && i < 3);
		return this->e[i];
	}
	T& operator[](int i) {
		assert(i >= 0 && i < 3);
		return this->e[i];
	}

	// Add
	Vector3& operator+=(const Vector3& v) {
		this->e[0] += v.x();
		this->e[1] += v.y();
		this->e[2] += v.z();
		return *this;
	}
	friend Vector3 operator+(const Vector3& u, const Vector3& v) {
		return Vector3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
	}

	// Subtract
	Vector3& operator-=(const Vector3& v) {
		this->e[0] -= v.x();
		this->e[1] -= v.y();
		this->e[2] -= v.z();
		return *this;
	}
	friend Vector3 operator-(const Vector3& u, const Vector3& v) {
		return Vector3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
	}

	// Multiply
	Vector3& operator*=(const T t) {
		this->e[0] *= t;
		this->e[1] *= t;
		this->e[2] *= t;
		return *this;
	}
	friend Vector3 operator*(const Vector3& u, const T t) {
		return Vector3(u.x() * t, u.y() * t, u.z() * t);
	}
	friend Vector3 operator*(const T t, const Vector3& u) {
		return Vector3(u.x() * t, u.y() * t, u.z() * t);
	}
	friend Vector3 operator*(const Vector3& u, const Vector3& v) {
		return Vector3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
	}

	// Divide
	Vector3& operator/=(const T t) {
		return *this *= (1 / t);
	}
	friend Vector3 operator/(const Vector3& u, const T t) {
		return Vector3(u.x() * (1 / t), u.y() * (1 / t), u.z() * (1 / t));
	}

	// Dot and Cross
	friend T dot(const Vector3& u, const Vector3& v) {
		return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
	}
	friend Vector3 cross(const Vector3& u, const Vector3& v) {
		return Vector3(
			u.y() * v.z() - u.z() * v.y(),
			u.z() * v.x() - u.x() * v.z(),
			u.x() * v.y() - u.y() * v.x()
			);
	}

	// Inverse
	Vector3 operator-() const {
		return Vector3(-this->e[0], -this->e[1], -this->e[2]);
	}

	// Compare
	friend bool operator==(const Vector3& u, const Vector3& v) {
		return (u.x() == v.x()) && (u.y() == v.y()) && (u.z() == v.z());
	}
	friend bool operator!=(const Vector3& u, const Vector3& v) {
		return (u.x() != v.x()) || (u.y() != v.y()) || (u.z() != v.z());
	}

	// Set
	Vector3& operator=(const Vector3& v) {
		this->e[0] = v.x();
		this->e[1] = v.y();
		this->e[2] = v.z();
		return *this;
	}

	// Normalize
	friend Vector3 normalize(const Vector3& u) {
		return u / u.magnitude();
	}

	// Length
	float magnitude() const {
		return std::sqrt(squaredMagnitude());
	}

	float squaredMagnitude() const {
		return this->e[0] * this->e[0] + this->e[1] * this->e[1] + this->e[2] * this->e[2];
	}

	bool nearZero() const {
		const float s = 1e-8f;
		return (std::fabs(this->e[0]) < s) && (std::fabs(this->e[1]) < s) && (std::fabs(this->e[2]) < s);
	}

private:
	T e[3];
};

// Common Types based on the Vector

using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

using Normal3i = Vector3<int>;
using Normal3f = Vector3<float>;
using Normal3d = Vector3<double>;

using Point3i = Vector3<int>;
using Point3f = Vector3<float>;
using Point3d = Vector3<double>;

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

using Normal2i = Vector2<int>;
using Normal2f = Vector2<float>;
using Normal2d = Vector2<double>;

using Point2i = Vector2<int>;
using Point2f = Vector2<float>;
using Point2d = Vector2<double>;