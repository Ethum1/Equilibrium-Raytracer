#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <ctime>
#include <iomanip>

#include "Image.h"
#include "BVH.h"
#include "Sampling.h"
#include "Camera.h"
#include "Scene.h"
#include "Primitive.h"
#include "Mesh.h"
#include "Integrator.h"

// Macros, Defines, and Enums

// File Extension (.png, .ppm, .jpg) will add more


// SceneData structs (not related to Scene enough to go in Scene.h)
struct SceneData {
	std::unordered_map<std::string, std::shared_ptr<Material>> materialMap; // Stores all of the materials in the scene ("name of material", pointer to materialObject)
	// Each material contains a pointer to a texture so no need for a texture storing map

	PIXSAMPLETYPE samplingMethod;

	INTEGRATORTYPE integrationMethod;

	int samples; // Number of samples

	int minBounces;

	float rrProb;

	std::shared_ptr<Scene> scene;
	std::shared_ptr<Integrator> integrator;

	Color backgroundColor;
};

// Equilibrium Class

class Equilibrium {
public:
	Equilibrium(const ImageData& _imageData, const CameraData& _cameraData, const SceneData& _sceneData) : imageData(_imageData), cameraData(_cameraData), sceneData(_sceneData) {}

	bool init();

	bool render();

	bool close();

private:
	ImageData imageData;
	CameraData cameraData;
	SceneData sceneData;

	// Shape: (could be a scene, or BVH_Node) in some cases Scene* is faster than BVH_Node*
	std::shared_ptr<Shape> scene; // Head of the tree (root)
	std::shared_ptr<Integrator> integrator;
	std::shared_ptr<Camera> camera;
};