#pragma once

#include <memory>

#include "Vector.h"
#include "Color.h"
#include "Ray.h"
#include "Scene.h"
#include "BVH.h"

// Path Integrator (Monte Carlo)
// Calculates the radiance with function "Li" (for Light incidence)

enum INTEGRATORTYPE {UNIFORM, IMPORTANCE, MULTIIMPORTANCE, NEE};

class Integrator {
public:
	virtual Color Li(const Ray& r, std::shared_ptr<Shape> scene, const Color& backgroundColor, int minDepth) const = 0;
};


// UDPTIntegrator = Uni-directional Path Tracing Integrator
// Supports: Uniform Sampling, Importance Sampling, Next-Event Estimation, and Multiple Importance Sampling
class UDPTIntegrator : public Integrator {
public:
	UDPTIntegrator() : type(UNIFORM), rrProbability(0.5), minDepth(0) {}
	UDPTIntegrator(int _minDepth, float _rrProbability) : minDepth(_minDepth), rrProbability(_rrProbability), type(UNIFORM) {}
	
	virtual Color Li(const Ray& r, std::shared_ptr<Shape> scene, const Color& backgroundColor, int minDepth) const override;
public:
	INTEGRATORTYPE type;
private:
	int minDepth;
	float rrProbability; // Russian Roulette Probability
};

extern long long int integrationCount;

// To be implemented in future versions

// Metropolis Algorithm Integrator
// Bidirectional Path Tracing