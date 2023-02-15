#include "Utilities.h"

// Definitions

bool Equilibrium::init() {
	// Initialize Scene and Camera (this->scene = _scene)

	if (this->cameraData.cameraType == PINHOLE) {
		this->camera = std::make_shared<PinholeCamera>(this->cameraData);
	}
	else if(this->cameraData.cameraType == THINLENS) {
		this->camera = std::make_shared<ThinLensCamera>(this->cameraData);
	}
	else if (this->cameraData.cameraType == ORTHOGRAPHIC) {
		this->camera = std::make_shared<OrthographicCamera>(this->cameraData);
	}

	this->integrator = this->sceneData.integrator;

	// Construct BVH
	this->scene = this->sceneData.scene; // Building goes on in the constructor

	// BVH_Node head(this->sceneData.scene->objects, this->sceneData.scene->objects.size());

	// this->scene = std::make_shared<BVH_Node>(head);

	return true;
}

bool Equilibrium::render() {
	// Initialize data: Format: (y=height) lines * (x=width)
	std::string data = "";

	// Coordinates for sampling
	std::vector<Vector2f> samples(this->sceneData.samples);


	// Sample According to the method
	if (this->sceneData.samplingMethod == RANDOM) {
		random(samples);
	}
	else if (this->sceneData.samplingMethod == JITTER) {

		if (!isPerfect(this->sceneData.samples)) {
			this->sceneData.samples = nearestPerfect(this->sceneData.samples);
		}
		jitter(samples);
	}
	else if (this->sceneData.samplingMethod == MULTIJITTER) {

		if (!isPerfect(this->sceneData.samples)) {
			this->sceneData.samples = nearestPerfect(this->sceneData.samples);
		}
		multiJitter(samples);
	}
	else if (this->sceneData.samplingMethod == NROOKS) {
		nrooks(samples);
	}

	std::clock_t start, end;

	start = std::clock();

	float clocksPerSec = static_cast<float>(1.0f / CLOCKS_PER_SEC);

	// Rendering Loop (recursive part)
	for (int i = this->imageData.imageHeight - 1; i >= 0; --i) {
		end = std::clock();
		std::cerr << "\r(Info): Render Progress: Scanlines Remaining: " << i 
			<< " : Time Elapsed: " << std::fixed << std::setprecision(3) << static_cast<float>(end - start) * clocksPerSec << std::flush;
		for (int j = 0; j < this->imageData.imageWidth; ++j) {
			Color pixelColor(0);

			for (int n = 0; n < this->sceneData.samples; ++n) {
				float x = (j + samples[n].x()) / (this->imageData.imageWidth - 1);
				float y = (i + samples[n].y()) / (this->imageData.imageHeight - 1);
				Ray camRay = this->camera->generateRay(x, y);
				pixelColor += this->integrator->Li(camRay, this->scene, this->sceneData.backgroundColor, 0); 
			}

			writeColorToFile(data, pixelColor, this->sceneData.samples, this->imageData.gamma);

			if (this->imageData.fileExtension == PNG) {
				data += (char)255;
			}
		}
	}

	std::cerr << std::endl << "(Info): Writing to Image File" << std::endl;

	// Output Data to Image (not ImageData)

	this->imageData.data = data;

	Image render(this->imageData);

	render.setMode(WRITE);
	render.writeImage();
	
	std::cerr << "(Info): Successfully Wrote Data" << std::endl;

	return true;
}

bool Equilibrium::close() {
	return true;
}