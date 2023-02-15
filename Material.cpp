#include "Material.h"

// Lambertian Diffuse

Color Lambertian::f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	// Convert wi and wo to the current local coordinates

	return (dot(x.sNormal, wi)) * this->albedo->value(x.uv, x.p) * EQMath::INVPI;
}

float Lambertian::PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	return (dot(x.sNormal, wi)) * EQMath::INVPI;
	// return 1.0f / (EQMath::PI * 4.0f);
}


Vector3f Lambertian::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	return uniformSampleSphere();
}

Vector3f Blackbody::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	return uniformSampleSphere();
}


/*
Vector3f Lambertian::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	Vector2f d = concentricSampleDisk();
	float z = std::sqrtf(std::max(0.0f, 1.0f - (d.x() * d.x()) - (d.y() - d.y())));
	return Vector3f(d.x(), z, d.y());
}
*/


/*
Vector3f Lambertian::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	return uniformSampleSphere();
}
*/

// Oren-Nayar Diffuse


Color OrenNayar::f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	Vector2f wiSphere(dot(x.uvw.W(), wi), dot(x.uvw.U(), wi));
	Vector2f woSphere(dot(x.uvw.W(), wo), dot(x.uvw.U(), wo));

	float alpha = std::max(wiSphere.x(), woSphere.x());
	float beta = std::min(wiSphere.x(), woSphere.x());

	float cosTerm = std::max(0.0f, std::cos(wiSphere.y() - woSphere.y()));

	return dot(x.sNormal, wi) * this->albedo->value(x.uv, x.p) * (this->A + (this->B * cosTerm * std::sinf(alpha) * std::tanf(beta))) * EQMath::INVPI;
}

/*
float OrenNayar::PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	Vector3f incident = wi.x() * x.uvw.U() + wi.y() * x.uvw.V() + wi.z() * x.uvw.W();
	Vector3f exitant = wo.x() * x.uvw.U() + wo.y() * x.uvw.V() + wo.z() * x.uvw.W();
	float angle = dot(x.sNormal, exitant);
	return (angle < 0.0f) ? 0.0f : angle * EQMath::INVPI;
}
*/

float OrenNayar::PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	return dot(x.sNormal, wi) * EQMath::INVPI;
}




Vector3f OrenNayar::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	Vector2f sample(EQMath::random(0.001f, 0.999f), EQMath::random(0.001f, 0.999f));

	sample.setX(std::acosf(std::sqrtf(sample.x())));
	sample.setY(2.0f * EQMath::PI * sample.y());

	return EQMath::sphereToXYZ(sample);
}



Vector2f concentricSampleDisk() {
	Vector2f u = EQMath::generateRandomVector2(-1.0f, 1.0f);
	Vector2f uOffset = EQMath::generateRandomVector2(-1.0f, 1.0f) + u - Vector2f(1.0f, 1.0f);

	if (uOffset.x() == 0.0f && uOffset.y() == 0.0f) {
		return Vector2f(0.0f, 0.0f);
	}

	float theta, r;
	
	if (std::abs(uOffset.x()) > std::abs(uOffset.y())) {
		r = uOffset.x();
		theta = (1.0f / 4.0f) * EQMath::PI * (uOffset.y() / uOffset.x());
	}
	else {
		r = uOffset.y();
		theta = (1.0f / 2.0f) * EQMath::PI - (1.0f / 4.0f) * EQMath::PI * (uOffset.x() / uOffset.y());
	}
	
	return r * Vector2f(std::cosf(theta), std::sinf(theta));
}

Vector3f uniformSampleHemisphere(const Vector3f& normal) {
	Vector2f u = EQMath::generateRandomVector2(-1.0f, 1.0f);
	
	float z = u.x();
	float r = std::sqrtf(std::max(0.0f, 1.0f - (z * z)));
	float phi = 2 * EQMath::PI * u.y();

	float x;
	float y;

	// r * std::cos(phi), z, r * std::sin(phi)
	return Vector3f(x, y, z);
}

/*
Vector3f uniformSampleSphere() {
	float U = EQMath::random(0.001f, 0.999f);
	float V = EQMath::random(0.001f, 0.999f);

	float theta = 2.0f * EQMath::PI * U;
	float phi = std::acosf(2.0f * V - 1.0f);

	float a = std::cosf(phi);
	
	float x = std::sqrtf(1.0f - a * a) * std::cosf(theta);
	float y = std::sqrtf(1.0f - a * a) * std::sinf(theta);
	float z = a;

	return Vector3f(x, y, z);
	
}
*/

Vector3f uniformSampleSphere() {
	

	Vector2f u = EQMath::generateRandomVector2(0.0f, 1.0f);

	float z = 2.0f * u.x() - 1.0f;
	float r = std::sqrtf(std::max(0.0f, 1.0f - (z * z)));
	float phi = 2.0f * EQMath::PI * u.y();

	return Vector3f(r * std::sinf(phi), r * std::cosf(phi), z);
}


/*
Vector3f OrenNayar::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	Vector2f u = EQMath::generateRandomVector2(0.0f, 0.999f);

	float m = 1.0f;

	float theta = std::acosf(std::powf(1.0f - u.x(), 1.0f / (1.0f + m)));
	float phi = 2.0f * EQMath::PI * u.y();

	float x = std::sinf(theta) * std::cosf(phi);
	float y = std::sinf(theta) * std::sinf(phi);
	float z = std::cosf(theta);

	return Vector3f(x, y, z);
}
*/


/*
Vector3f OrenNayar::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	Vector2f u = concentricSampleDisk();

	return Vector3f(u.x(), u.y(), std::max(0.0f, std::sqrtf(1.0f - (u.x() * u.x()) - (u.y() + u.y()))));
}
*/

// Ideal Specular 100% Reflector

Color Specular::f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	return this->albedo->value(x.uv, x.p);
}

float Specular::PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	return 1.0f;
}

Vector3f Specular::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	return reflect(wi, normal) + this->roughness * EQMath::randomInUnitSphere();
}

Vector3f Specular::reflect(const Vector3f& omega, const Vector3f& normal) const {
	return Vector3f(omega - 2 * (dot(omega, normal) * normal));
}

Color Blackbody::Le(const Point3f& p, const Vector2f& uv) const {
	// return Color(0.0f);
	return this->color * this->intensity;
}

Color Blackbody::f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	return (dot(x.sNormal, wi)) * Color(1.0f, 1.0f, 1.0f);
	// return (1.0f / dot(x.sNormal, wi)) * Color(1.0f, 1.0f, 1.0f);
}


/*
float Blackbody::PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	return 0.1f;
}
*/


float Blackbody::PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const {
	return dot(x.sNormal, wi) * EQMath::INVPI;
	// return 2.0f * EQMath::PI;
}

/*
Vector3f Blackbody::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	// Blackbodies absorb all light so there is no sampling
	return Vector3f(1.0f, 1.0f, 0.0f);
}
*/

/*
Vector3f Blackbody::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	Vector2f sample(EQMath::random(0.001f, 0.999f), EQMath::random(0.001f, 0.999f));

	sample.setX(std::acosf(std::sqrtf(sample.x())));
	sample.setY(2.0f * EQMath::PI * sample.y());

	return EQMath::sphereToXYZ(sample);
	return EQMath::sphereToXYZ(sample);
}
*/

/*
Vector3f Blackbody::sampleBSDF(const Vector3f& wi, const Vector3f& normal) const {
	Vector2f d = concentricSampleDisk();
	float z = std::sqrtf(std::max(0.0f, 1.0f - (d.x() * d.x()) - (d.y() - d.y())));
	return Vector3f(d.x(), z, d.y());
}
*/




Color Blackbody::temperatureToRGB(float temperature) const {
	temperature /= 100.0f;

	float r, g, b;

	// Calculate Red

	if (temperature <= 66.0f) {
		r = 255.0f;
	}
	else {
		r = temperature - 60.0f;
		r = 329.698737446f * (std::pow(r, -0.1332047592f));
		if (r < 0.0f) {
			r = 0.0f;
		}
		if (r > 255.0f) {
			r = 255.0f;
		}
	}

	// Calculate Green

	if (temperature <= 66.0f) {
		g = temperature;
		g = 99.4708025861f * std::log(g) - 161.1195681661f;
		if (g < 0.0f) {
			g = 0.0f;
		}
		if (g > 255.0f) {
			g = 255.0f;
		}
	}
	else {
		g = temperature - 60.0f;
		g = 288.1221695283f * (std::pow(g, -0.0755148492f));
		if (g < 0.0f) {
			g = 0.0f;
		}
		if (g > 255.0f) {
			g = 255.0f;
		}
	}

	// Calculate Blue

	if (temperature >= 66.0f) {
		b = 255.0f;
	}
	else {
		if (temperature <= 19.0f) {
			b = 0.0f;
		}
		else {
			b = temperature - 10.0f;
			b = 138.5177312231f * std::log(b) - 305.0447927307f;
			if (b < 0.0f) {
				b = 0.0f;
			}
			if (b > 255.0f) {
				b = 255.0f;
			}
		}
	}

	return Color(r / 256.0f, g / 256.0f, b / 256.0f);
}