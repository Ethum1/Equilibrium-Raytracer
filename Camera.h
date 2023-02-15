#pragma once

#include "Color.h"
#include "Normal.h"
#include "Ray.h"
#include "Math.h"

enum CAMERATYPE {PINHOLE, ORTHOGRAPHIC, THINLENS};

// Camera Data
struct CameraData {
	// Will include stuff here
	Point3f origin; 
	Vector3f direction; // Normalized
	Vector3f vup;
	float fov; // Vertical FOV
	float aspectRatio;
	float aperture;
	float focusDistance; // Distance between perspective origin (eye) and focus plane
	float time0, time1; // Shutter Open / Close

	CAMERATYPE cameraType;
};

// Main Abstract Camera Class
class Camera {
public:
	Camera() : aperture(0.0f), shutterOpen(0.0f), shutterClose(0.0f) {}
	Camera(const CameraData& camData);

	virtual Ray generateRay(float x, float y) const = 0; // (x,y) are the sample coordinates on the image plane
protected:
	Point3f origin; // Perspective Origin / Eye
	Point3f lowerLeftCorner; // UV Coordinates for camray are generated from here (0,0) on the image plane
	Vector3f horizontal; // Basis Vector for image coordinate 
	Vector3f vertical;
	ONBf camONB; // For Gazing Direction
	float aperture; // For Depth of Field
	float shutterOpen, shutterClose; // For Motion Blur
};

// Subclass Types of Cameras

// Most Basic Perspective Camera: Pinhole
class PinholeCamera : public Camera {
public:
	PinholeCamera(const CameraData& camData);

	virtual Ray generateRay(float x, float y) const override;
};

// Orthographic Camera
class OrthographicCamera : public Camera {
public:
	OrthographicCamera(const CameraData& camData);

	virtual Ray generateRay(float x, float y) const override;
};


// Realistic Camera: Depth of Field & Motion Blur
class ThinLensCamera : public Camera {
public:
	ThinLensCamera(const CameraData& camData);

	virtual Ray generateRay(float x, float y) const override;
};