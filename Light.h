#pragma once

#include "Ray.h"
#include "Vector.h"
#include "Color.h"
#include "Material.h"
#include "Texture.h"

// A Light is a specific type of object / entity in space. This is because it is marked for light source sampling. There are four types of lights:
// - Directional Light (ex: sun, infinitely far away lights no distance attenuation)
// - Point Light (infinitely small light and emits light in all directions, light source sampling is impossible with this)
// - Area Light (shape that has an emissive surface, a blackbody)
// - Spotlight (Hollywood)
// - Environment Light (Infinite area light)

// Lights have a very similar interface to materials ,

enum LIGHTTYPES {DIRECTIONAL, POINT, AREA, SPOTLIGHT, ENVIRONMENT};

class Light {
public:
	virtual double intensity() const = 0; // Gives a non-zero multiplier for light intensity

	virtual Color f() const = 0;

	virtual float PDF() const = 0; 

	virtual Vector3f sampleLight() const = 0;
};

// Infinitely small point (x, y, z) coordinate. Brightness governed by intensity. Omnidirectional
class PointLight : public Light {

};

// Contains a pointer to a shape, area light 
class AreaLight : public Light {

};

class SpotLight : public Light {

};

class EnvironmentLight : public Light {

};