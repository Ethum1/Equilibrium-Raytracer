#pragma once

#include "Vector.h"

// Orthonormal Basis (three vectors)
template<typename T>
class ONB {
public:
	ONB() {}
	ONB(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c) : u(a), v(b), w(c) {}

	// Getters and Setters

	void set(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c) { this->u = a; this->v = b; this->w = c; }
	Vector3<T> U() const { return this->u; }
	Vector3<T> V() const { return this->v; }
	Vector3<T> W() const { return this->w; }

	void setU(const Vector3<T>& a) { this->u = a; }
	void setV(const Vector3<T>& a) { this->v = a; }
	void setW(const Vector3<T>& a) { this->w = a; }

	// Build from one vector

	void buildFromU(const Vector3<T>& u) {
		Vector3<T> n(1.0f, 0.0f, 0.0f); // Implicit conversion to double
		Vector3<T> m(0.0f, 1.0f, 0.0f);

		this->u = normalize(u);
		this->v = cross(this->u, n);

		if (this->v.magnitude() < 0.01f) {
			this->v = cross(this->u, m);
		}
		this->w = cross(this->u, this->v);
	}
	void buildFromV(const Vector3<T>& v) {
		Vector3<T> n(1.0f, 0.0f, 0.0f); // Implicit conversion to double
		Vector3<T> m(0.0f, 1.0f, 0.0f); // Could use static_cast<T> but nah

		this->v = normalize(v);
		this->u = cross(this->v, n);

		if (this->u.squaredMagnitude() < 0.01f) {
			this->u = cross(this->v, m);
		}
		this->w = cross(this->u, this->v);
	}
	void buildFromW(const Vector3<T>& w) {
		Vector3<T> n(1.0f, 0.0f, 0.0f);
		Vector3<T> m(0.0f, 1.0f, 0.0f);

		this->w = normalize(w);
		this->u = cross(this->w, n);
		if (this->u.magnitude() < 0.01f) {
			this->u = cross(this->w, m);
		}
		this->v = cross(this->w, this->u);
	}

	// Build from two vectors

	void buildFromUV(const Vector3<T>& u, const Vector3<T>& v) {
		this->u = normalize(u);
		this->w = normalize(cross(u, v));
		this->v = cross(this->w, this->u);
	}
	void buildFromVU(const Vector3<T>& v, const Vector3<T>& u) {
		this->v = normalize(v);
		this->w = normalize(cross(u, v));
		this->u = cross(this->v, this->w);
	}

	void buildFromUW(const Vector3<T>& u, const Vector3<T>& w) {
		this->u = normalize(u);
		this->v = normalize(cross(w, u));
		this->w = cross(this->u, this->v);
	}
	void buildFromWU(const Vector3<T>& w, const Vector3<T>& u) {
		this->w = normalize(w);
		this->v = normalize(cross(w, u));
		this->u = cross(this->v, this->w);
	}

	void buildFromVW(const Vector3<T>& v, const Vector3<T>& w) {
		this->v = normalize(v);
		this->u = normalize(cross(v, w));
		this->w = cross(this->u, this->v);
	}
	void buildFromWV(const Vector3<T>& w, const Vector3<T>& v) {
		this->w = normalize(w);
		this->u = normalize(cross(v, w));
		this->v = cross(this->w, this->u);
	}

	// Compare
	friend bool operator==(const ONB& a, const ONB& b) {
		return (a.U() == b.U()) && (a.V() == b.V()) && (a.W() == b.W());
	}
	friend bool operator!=(const ONB& a, const ONB& b) {
		return (a.U() != b.U()) || (a.V() != b.V()) || (a.W() != b.W());
	}

private:
	Vector3<T> u, v, w;
};

// Common Types

using ONBd = ONB<double>;
using ONBi = ONB<int>;
using ONBf = ONB<float>;