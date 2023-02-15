#include "Camera.h"

Camera::Camera(const CameraData& camData) {
	float theta = EQMath::degtorad(camData.fov);
	float h = std::tan(theta / 2.0f);
	float viewportHeight = 2.0f * h;
	float viewportWidth = camData.aspectRatio * viewportHeight;

	this->camONB = ONBf();
	this->camONB.buildFromWV(camData.direction, camData.vup);

	this->origin = camData.origin;
	this->horizontal = camData.focusDistance * viewportWidth * this->camONB.U();
	this->vertical = camData.focusDistance * viewportHeight * this->camONB.V();
	this->lowerLeftCorner = this->origin - (this->horizontal / 2) - (this->vertical / 2) - camData.focusDistance * this->camONB.W();
	
	this->aperture = camData.aperture;
	this->shutterOpen = camData.time0;
	this->shutterClose = camData.time1;
}

PinholeCamera::PinholeCamera(const CameraData& camData) {
	float theta = EQMath::degtorad(camData.fov);
	float h = std::tan(theta / 2.0f);
	float viewportHeight = 2.0f * h;
	float viewportWidth = camData.aspectRatio * viewportHeight;

	ONBf temp;
	temp.buildFromWV(camData.direction, camData.vup);

	this->camONB = temp;

	this->origin = camData.origin;
	this->horizontal = viewportWidth * this->camONB.U();
	this->vertical = viewportHeight * this->camONB.V();
	this->lowerLeftCorner = this->origin - (this->horizontal / 2.0f) - (this->vertical / 2.0f) - this->camONB.W();

	this->shutterOpen = camData.time0;
	this->shutterClose = camData.time1;
}

Ray PinholeCamera::generateRay(float x, float y) const {
	return Ray(this->origin, this->lowerLeftCorner + x * this->horizontal + y * this->vertical - this->origin, EQMath::random(this->shutterOpen, this->shutterClose), 1.0f);
}

OrthographicCamera::OrthographicCamera(const CameraData& camData) {
	// No perspective so no viewport height/width nor FOV

	this->camONB = ONBf();
	this->camONB.buildFromWV(camData.direction, camData.vup);

	this->origin = camData.origin;
	this->horizontal = this->camONB.U();
	this->vertical = this->camONB.V();
	this->lowerLeftCorner = this->origin - (this->horizontal / 2.0f) - (this->vertical / 2.0f) - this->camONB.W();

	this->shutterOpen = camData.time0;
	this->shutterClose = camData.time0;
}

Ray OrthographicCamera::generateRay(float x, float y) const {
	return Ray(this->origin, this->lowerLeftCorner + x * this->horizontal + y * this->vertical - origin, EQMath::random(this->shutterOpen, this->shutterClose), 1.0f);
}

ThinLensCamera::ThinLensCamera(const CameraData& camData) {
	float theta = EQMath::degtorad(camData.fov);
	float h = std::tan(theta / 2.0f);
	float viewportHeight = 2.0f * h;
	float viewportWidth = camData.aspectRatio * viewportHeight;

	ONBf temp;
	temp.buildFromWV(camData.direction, camData.vup);

	this->camONB = temp;

	this->origin = camData.origin;
	this->horizontal = camData.focusDistance * viewportWidth * this->camONB.U();
	this->vertical = camData.focusDistance * viewportHeight * this->camONB.V();
	this->lowerLeftCorner = this->origin - (this->horizontal / 2.0f) - (this->vertical / 2.0f) - camData.focusDistance * this->camONB.W();

	this->aperture = camData.aperture;
	this->shutterOpen = camData.time0;
	this->shutterClose = camData.time1;
}

Ray ThinLensCamera::generateRay(float x, float y) const {
	Vector2f lensSampleCoordinates = (this->aperture / 2.0f) * EQMath::randomInUnitDisk();
	Vector3f offset = this->camONB.U() * lensSampleCoordinates.x() + this->camONB.V() * lensSampleCoordinates.y();

	return Ray(this->origin + offset, this->lowerLeftCorner + x * this->horizontal + y * this->vertical - this->origin, EQMath::random(this->shutterOpen, this->shutterClose), 1.0f);
}