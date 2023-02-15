#pragma once

#include <memory>

#include "Ray.h"
#include "Vector.h"
#include "Normal.h"
#include "Color.h"
#include "Texture.h"

#define DIFFUSE 1
#define BLACKBODY 2
#define SPECULAR 3
#define GLOSSY 4
#define DIELECTRIC 5


class Material;

class Interaction {
public:
	// Normals ALWAYS point opposite of the incident ray

	Point3f p; // Point of intersection
	float t; // Time of Intersection
	Vector3f sNormal; // Surface Normal (will be useful when normal mapping)
	Vector3f gNormal; // Geometric Normal (standard, if no normal map then sNormal = gNormal)
	std::shared_ptr<Material> material; // Will be replaced by material
	bool frontFace; // Is ray coming from inside or outside of object
	ONBf uvw; // Local coordinate system
	Vector2f uv; // UV Coordinates for texture mapping

	inline void setNormal(const Ray& r, const Vector3f& outNormal) {
		if (dot(r.direction(), outNormal) > 0.0f) {
			// Ray is inside
			this->gNormal = -outNormal;
			this->sNormal = -outNormal;
			this->frontFace = false;
			this->uvw.buildFromW(this->sNormal);
		}
		else {
			// Ray is outside
			this->gNormal = outNormal;
			this->sNormal = outNormal;
			this->frontFace = true;
			this->uvw.buildFromW(this->sNormal);
		}
	}
};

// Material = BSDF
class Material {
public:
	virtual Color Le(const Point3f& p, const Vector2f& uv) const { return Color(0); }; // Emitted Radiance

	virtual Color f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const = 0; // Returns the explicit ratio of incident radiance to exitant radiance (BSDF)

	virtual float PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const = 0; // PDF returns the PDF lmao

	virtual Vector3f sampleBSDF(const Vector3f& wi, const Vector3f& normal) const = 0; // Omega is outgoing vector

	virtual int getType() const = 0;
public:
	int type = 0; // This needs to be initialized
	float ior; // Index of Refraction of the material (at the boundary and subsurface)
};

class Lambertian : public Material {
public:
	Lambertian() : type(DIFFUSE) {}
	Lambertian(Color c) : albedo(std::make_shared<SolidTexture>(c)), type(DIFFUSE) {}
	Lambertian(std::shared_ptr<Texture> texture) : albedo(texture), type(DIFFUSE) {}

	// Le is already implemented in the abstract class

	virtual Color f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override;

	virtual float PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override;

	virtual Vector3f sampleBSDF(const Vector3f& wi, const Vector3f& normal) const override;

	virtual int getType() const override { return this->type; }

public:
	int type;
private:
	std::shared_ptr<Texture> albedo;
};


class OrenNayar : public Material {
public:
	OrenNayar() : type(DIFFUSE), A(0), B(0), roughness(0) {}
	OrenNayar(Color c, float _roughness = 0.0f) : albedo(std::make_shared<SolidTexture>(c)), type(DIFFUSE), roughness(_roughness) {
		this->A = 1.0f - 0.5f * (roughness / (roughness + 0.33f));
		this->B = 0.45f * (roughness / (roughness + 0.09f));
	}

	OrenNayar(std::shared_ptr<Texture> texture, float _roughness) : albedo(texture), type(DIFFUSE), roughness(_roughness) {
		this->A = 1.0f - 0.5f * (roughness / (roughness + 0.33f));
		this->B = 0.45f * (roughness / (roughness + 0.09f));
	}

	virtual Color f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override;

	virtual float PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override;

	virtual Vector3f sampleBSDF(const Vector3f& wi, const Vector3f& normal) const override;
	
	virtual int getType() const override { return this->type; }

public:
	int type;
private:
	std::shared_ptr<Texture> albedo;
	float roughness;
	float A; // A and B are parameters for the Oren-Nayar model
	float B;
};


// Ideal Specular (Reflects 100% + Roughness)
class Specular : public Material {
public:
	Specular() {}
	Specular(Color c, float _roughness = 0.0f) : albedo(std::make_shared<SolidTexture>(c)), type(SPECULAR), roughness(_roughness) {}
	Specular(std::shared_ptr<Texture> texture, float _roughness = 0.0f) : albedo(texture), type(SPECULAR), roughness(_roughness) {}

	virtual Color f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override; // Return 100%, because this is a 100% reflector

	virtual float PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override; // PDF of dirac delta function = 1

	virtual Vector3f sampleBSDF(const Vector3f& wi, const Vector3f& normal) const override; // Generate Reflected Ray (Rout = Rin - 2(Rin dot N) * N

	virtual int getType() const override { return this->type; }
private:
	Vector3f reflect(const Vector3f& omega, const Vector3f& normal) const;
public:
	int type;
private:
	std::shared_ptr<Texture> albedo;
	float roughness;
};

// Blackbody Radiating Light (this is different from Area Light, because this does not support Light Source Sampling)
class Blackbody : public Material {
public:
	Blackbody() : type(BLACKBODY) {}
	Blackbody(float _intensity, float _temperature) : intensity(_intensity), temperature(_temperature), type(BLACKBODY) { this->color = temperatureToRGB(this->temperature); }

	virtual Color Le(const Point3f& p, const Vector2f& uv) const override;

	virtual Color f(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override; // Return 100%, because this is a 100% reflector

	virtual float PDF(const Vector3f& wi, const Vector3f& wo, const Interaction& x) const override; // PDF of dirac delta function = 1

	virtual Vector3f sampleBSDF(const Vector3f& wi, const Vector3f& normal) const override; // Generate Reflected Ray (Rout = Rin - 2(Rin dot N) * N

	virtual int getType() const override { return this->type; }
public:
	int type;

private:
	Color temperatureToRGB(float temperature) const;

private:
	float intensity;
	float temperature;
	Color color;
};

// Glossy is a Specular-Diffuse (mixture)
class Glossy : public Material {
public:
	
};

// Conductor (follows Fresnel reflectance for conductors)
class Metal : public Material {

};

// Dielectric (follows Fresnel reflectance for dielectrics)
class Dielectric : public Material {

};

Vector2f concentricSampleDisk();

Vector3f uniformSampleHemisphere(const Vector3f& normal);

Vector3f uniformSampleSphere();