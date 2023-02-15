#include "Utilities.h"


void cornelliusBox(SceneData& sceneData, ImageData& imageData, CameraData& cameraData) {
	imageData.aspectRatio = 1.0f;
	imageData.exposure = 1.0f;
	imageData.gamma = 1.0f;
	imageData.imageWidth = 600;
	imageData.imageHeight = static_cast<int>((1.0f / imageData.aspectRatio) * imageData.imageWidth);
	imageData.fileExtension = PNG;
	imageData.filePath = "Render.png";
	imageData.data = "";

	cameraData.aperture = 0.001f;
	cameraData.aspectRatio = 1.0f;
	cameraData.direction = Vector3f(0.0f, 0.0f, -1.0f);
	cameraData.vup = Vector3f(0.0f, 1.0f, 0.0f);
	cameraData.focusDistance = 1.0f;
	cameraData.fov = 40.0f;
	cameraData.origin = Point3f(278.0f, 278.0f, -750.0f);
	cameraData.time0 = 0.0f;
	cameraData.time1 = 0.0f;
	cameraData.cameraType = PINHOLE;

	sceneData.samples = 100;
	sceneData.minBounces = 6;
	sceneData.rrProb = 0.85f;
	sceneData.samplingMethod = MULTIJITTER;
	sceneData.integrationMethod = UNIFORM;
	sceneData.backgroundColor = Color(0.05f);

	if (sceneData.integrationMethod == UNIFORM) {
		sceneData.integrator = std::make_shared<UDPTIntegrator>(sceneData.minBounces, sceneData.rrProb);
	}

	auto red = std::make_shared<Lambertian>(Color(0.65f, 0.05f, 0.05f));
	auto green = std::make_shared<Lambertian>(Color(0.12f, 0.45f, 0.15f));
	auto white = std::make_shared<Lambertian>(Color(0.73f, 0.73f, 0.73f));
	auto black = std::make_shared<Lambertian>(Color(0.05f, 0.05f, 0.05f));
	auto blue = std::make_shared<Lambertian>(Color(0.15f, 0.15f, 0.85f));
	auto light = std::make_shared<Blackbody>(50.0f, 5500.0f);
	auto light1 = std::make_shared<Blackbody>(1.05f, 5500.0f);
	auto metal = std::make_shared<Specular>(Color(0.95f), 0.5f);

	Scene world;

	// world.add(std::make_shared<Sphere>(Point3f(0.0f, -100.5f, 1.0f), 100.0f, sceneData.materialMap.at("red")));
	// world.add(std::make_shared<Sphere>(Point3f(0.0f, 0.0f, 1.0f), 0.5f, sceneData.materialMap.at("light")));

	world.add(std::make_shared<Rectangle>(Point3f(555.0f, 0.0f, 0.0f), Point3f(555.0f, 0.0f, 555.0f), Point3f(555.0f, 555.0f, 0.0f), red)); // Left Wall
	world.add(std::make_shared<Rectangle>(Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f, 0.0f, 555.0f), Point3f(0.0f, 555.0f, 0.0f), green)); // Right Wall

	//world.add(std::make_shared<Rectangle>(Point3f(0.0f, 554.0f, 550.0f), Point3f(555.0f, 554.0f, 550.0f), Point3f(555.0f, 554.0f, 556.0f), light)); // Light
	//world.add(std::make_shared<Rectangle>(Point3f(-1.0f, 554.0f, 0.0f), Point3f(-1.0f, 554.0f, 555.0f), Point3f(5.0f, 554.0f, 0.0f), light)); // Light
	//world.add(std::make_shared<Rectangle>(Point3f(556.0f, 554.0f, 0.0f), Point3f(556.0f, 554.0f, 555.0f), Point3f(550.0f, 554.0f, 0.0f), light)); // Light

	// world.add(std::make_shared<Rectangle>(Point3f(0.0f, 1.0f, 550.0f), Point3f(555.0f, 1.0f, 550.0f), Point3f(555.0f, 1.0f, 556.0f), sceneData.materialMap.at("light"))); // Light
	//world.add(std::make_shared<Rectangle>(Point3f(-1.0f, 1.0f, 0.0f), Point3f(-1.0f, 1.0f, 555.0f), Point3f(5.0f, 1.0f, 0.0f), light)); // Light
	//world.add(std::make_shared<Rectangle>(Point3f(556.0f, 1.0f, 0.0f), Point3f(556.0f, 1.0f, 555.0f), Point3f(550.0f, 1.0f, 0.0f), light)); // Light

	//world.add(std::make_shared<Rectangle>(Point3f(550.0f, 0.0f, 554.0f), Point3f(550.0f, 556.0f, 554.0f), Point3f(556.0f, 0.0f, 554.0f), light)); // Light
	//world.add(std::make_shared<Rectangle>(Point3f(5.0f, 0.0f, 554.0f), Point3f(5.0f, 556.0f, 554.0f), Point3f(-1.0f, 0.0f, 554.0f), light)); // Light

	world.add(std::make_shared<Rectangle>(Point3f(213.0f, 554.0f, 227.0f), Point3f(343.0f, 554.0f, 227.0f), Point3f(213.0f, 554.0f, 332.0f), light)); // Light

	world.add(std::make_shared<Rectangle>(Point3f(0.0f, 0.0f, 0.0f), Point3f(555.0f, 0.0f, 0.0f), Point3f(0.0f, 0.0f, 555.0f), white)); // Bottom
	world.add(std::make_shared<Rectangle>(Point3f(0.0f, 555.0f, 0.0f), Point3f(555.0f, 555.0f, 0.0f), Point3f(0.0f, 555.0f, 555.0f), white)); // Top
	world.add(std::make_shared<Rectangle>(Point3f(0.0f, 0.0f, 555.0f), Point3f(555.0f, 0.0f, 555.0f), Point3f(0.0f, 555.0f, 555.0f), white)); // Back

	//world.add(std::make_shared<Sphere>(Point3f(278.0f, 240.0f, 278.0f), 50.0f, light));
	//world.add(std::make_shared<Rectangle>(Point3f(250.0f, 220.0f, 227.0f), Point3f(250.0f, 220.0f, 332.0f), Point3f(250.0f, 320.0f, 227.0f), light));

	sceneData.scene = std::make_shared<Scene>(world);
}

void cornellius2Box(SceneData& sceneData, ImageData& imageData, CameraData& cameraData) {
	imageData.aspectRatio = 1.0f;
	imageData.exposure = 1.0f;
	imageData.gamma = 1.0f;
	imageData.imageWidth = 600;
	imageData.imageHeight = static_cast<int>((1.0f / imageData.aspectRatio) * imageData.imageWidth);
	imageData.fileExtension = PNG;
	imageData.filePath = "Render.png";
	imageData.data = "";

	cameraData.aperture = 0.001f;
	cameraData.aspectRatio = 1.0f;
	cameraData.direction = Vector3f(0.0f, 0.0f, -1.0f);
	cameraData.vup = Vector3f(0.0f, 1.0f, 0.0f);
	cameraData.focusDistance = 1.0f;
	cameraData.fov = 40.0f;
	cameraData.origin = Point3f(278.0f, 278.0f, -750.0f);
	cameraData.time0 = 0.0f;
	cameraData.time1 = 0.0f;
	cameraData.cameraType = PINHOLE;

	sceneData.samples = 1600;
	sceneData.minBounces = 50;
	sceneData.rrProb = 0.00f;
	sceneData.samplingMethod = MULTIJITTER;
	sceneData.integrationMethod = UNIFORM;
	sceneData.backgroundColor = Color(0.05f);

	if (sceneData.integrationMethod == UNIFORM) {
		sceneData.integrator = std::make_shared<UDPTIntegrator>(sceneData.minBounces, sceneData.rrProb);
	}

	auto red = std::make_shared<Lambertian>(Color(0.65f, 0.05f, 0.05f));
	auto green = std::make_shared<Lambertian>(Color(0.12f, 0.45f, 0.15f));
	auto white = std::make_shared<Lambertian>(Color(0.73f, 0.73f, 0.73f));
	auto black = std::make_shared<Lambertian>(Color(0.05f, 0.05f, 0.05f));
	auto blue = std::make_shared<Lambertian>(Color(0.15f, 0.15f, 0.85f));
	auto light = std::make_shared<Blackbody>(8.0f, 5500.0f);
	auto light1 = std::make_shared<Blackbody>(1.05f, 5500.0f);
	auto metal = std::make_shared<Specular>(Color(0.95f), 0.00f);
	auto metal2 = std::make_shared<Specular>(Color(0.95f), 0.54f);

	Scene world;

	world.add(std::make_shared<YZRect>(555.0f, 0.0f, 0.0f, 555.0f, 555.0f, red)); // Left
	world.add(std::make_shared<YZRect>(0.0f, 0.0f, 0.0f, 555.0f, 555.0f, green)); // Right
	world.add(std::make_shared<XZRect>(554.0f, 113.0f, 127.0f, 443.0f, 432.0f, light));
	world.add(std::make_shared<XZRect>(555.0f, 0.0f, 0.0f, 555.0f, 555.0f, white)); // Top
	world.add(std::make_shared<XZRect>(0.0f, 0.0f, 0.0f, 555.0f, 555.0f, white)); // Bottom
	world.add(std::make_shared<XYRect>(555.0f, 0.0f, 0.0f, 555.0f, 555.0f, white)); // Back
	world.add(std::make_shared<Sphere>(Point3f(150.0f, 250.0f, 250.0f),100.0f, blue));
	world.add(std::make_shared<Sphere>(Point3f(350.0f, 100.0f, 450.0f), 50.0f, black));
	world.add(std::make_shared<YZRect>(555.0f, 50.0f, 0.0f, 505.0f, 555.0f, metal)); // Mirror


	sceneData.scene = std::make_shared<Scene>(world);
}


void twoSpheres(SceneData& sceneData, ImageData& imageData, CameraData& cameraData) {
	imageData.aspectRatio = 1.0f;
	imageData.exposure = 1.0f;
	imageData.gamma = 1.0f;
	imageData.imageWidth = 600;
	imageData.imageHeight = static_cast<int>((1.0f / imageData.aspectRatio) * imageData.imageWidth);
	imageData.fileExtension = PNG;
	imageData.filePath = "Render.png";
	imageData.data = "";

	cameraData.aperture = 0.001f;
	cameraData.aspectRatio = 1.0f;
	cameraData.direction = Vector3f(0.0f, 0.0f, -1.0f);
	cameraData.vup = Vector3f(0.0f, 1.0f, 0.0f);
	cameraData.focusDistance = 1.0f;
	cameraData.fov = 90.0f;
	cameraData.origin = Point3f(0.0f, 0.0f, 0.0f);
	cameraData.time0 = 0.0f;
	cameraData.time1 = 0.0f;
	cameraData.cameraType = PINHOLE;

	sceneData.samples = 4;
	sceneData.minBounces = 4;
	sceneData.rrProb = 0.8f;
	sceneData.samplingMethod = MULTIJITTER;
	sceneData.integrationMethod = UNIFORM;
	sceneData.backgroundColor = Color(0.5f);

	if (sceneData.integrationMethod == UNIFORM) {
		sceneData.integrator = std::make_shared<UDPTIntegrator>(sceneData.minBounces, sceneData.rrProb);
	}

	auto red = std::make_shared<Lambertian>(Color(0.65f, 0.05f, 0.05f));
	auto green = std::make_shared<Lambertian>(Color(0.12f, 0.45f, 0.15f));
	auto white = std::make_shared<Lambertian>(Color(0.73f, 0.73f, 0.73));
	auto blue = std::make_shared<Lambertian>(Color(0.15f, 0.15f, 0.85f));
	auto light = std::make_shared<Blackbody>(0.5f, 3500.0f);

	Scene world;
	world.add(std::make_shared<Sphere>(Point3f(0.0f, 0.0f, 1.0f), 0.5f, light));
	world.add(std::make_shared<Sphere>(Point3f(0.0f, -100.5f, -1.0f), 100.0f, white));
	// world.add(std::make_shared<XZRect>(0.5f, -20.0f, -20.0f, 20.0f, 20.0f, white));
	world.add(std::make_shared<Sphere>(Point3f(0.0f, 100.5f, -1.0f), 100.0f, white));

	sceneData.scene = std::make_shared<Scene>(world);
}

int main(int argc, char* argv[]) {

	std::cerr << "(Info): Starting Image Data Collection" << std::endl;

	// Initialize the Renderer
	ImageData imageData;
	

	std::cerr << "(Info): Finished Image Data Collection, Starting Camera Data Collection" << std::endl;

	CameraData cameraData;
	

	std::cerr << "(Info): Finished Camera Data Collection, Starting Scene Data Collection" << std::endl;

	SceneData sceneData;
	
	twoSpheres(sceneData, imageData, cameraData);

	std::cerr << "(Info): Finished Scene Data Collection, Initializing Renderer" << std::endl;

	Equilibrium equilibrium(imageData, cameraData, sceneData);

	std::clock_t start, end;

	start = clock();

	if (equilibrium.init() == false) {
		std::cerr << "Error: (Failed to Initialize Equilibrium Render Object): Returning EXIT_FAILURE" << std::endl;
		return EXIT_FAILURE;
	}

	end = clock();

	std::cerr << "(Info): Renderer Initialized Successfully:\n";
	std::cerr << "- BVH Constructed\n";
	std::cerr << "Time Taken: " << std::fixed << std::setprecision(5) << static_cast<float>(end - start) / static_cast<float>(CLOCKS_PER_SEC) << std::endl;

	// Render (Raytracing)

	std::cerr << "(Info): Starting Rendering Process" << std::endl;

	if (equilibrium.render() == false) {
		std::cerr << "Error: (Malfunction in Main Render Loop): Returning EXIT_FAILURE" << std::endl;
		return EXIT_FAILURE;
	}

	std::cerr << "(Info): Finished Rendering Process" << std::endl;

	std::cerr << integrationCount << std::endl;

	// De-initialize the Renderer (Deallocate memory)

	std::cerr << "(Info): Starting Renderer Deconstruction" << std::endl;

	if (equilibrium.close() == false) {
		std::cerr << "Error: (Failed to Deallocate Equilibrium Render Object): Returning EXIT_FAILURE" << std::endl;
		return EXIT_FAILURE;
	}

	std::cerr << "(Info): Finished Renderer Deconstruction Safely" << std::endl;

	return EXIT_SUCCESS;
}