# Equilibrium Raytracer

A ray tracing graphics engine from scratch in C++ for the IB personal project.

## Disclaimer

Since this is just an initial commit and not even close to the final code, there is a lot of code cleanup necessary. Comments are scattered for debugging purposes. The functionality is the same, but the code itself is not in fully presentable form.

## Usage

This engine uses .png files. If you clone the repository you should be able to run it without conflicts since there are no external dependencies. This project was tested on Visual Studio on Windows using the MSVC compiler, and I have not tested it on other compilers or systems since I am just submitting this code since the report is due, although the actual engine is not 100% complete but sufficiently usable. 

The rendered image should be updated in the Render.png file.

The main entry point is the main.cpp file. First note that there are a few data objects to be known:

* imageData (Aspect Ratio, Rendered Image Size XY, Exposure, Gamma, File Extension, and Target File Name: just keep it Render.png)
* cameraData (Aperture, Aspect Ratio, Viewing Vector Direction, Camera Type, Focus, and FOV)
* sceneData (Min Bounces, Samples, Sampling Method, Integration Method)

To edit the scene: go to the main.cpp file and find the global function
````cpp
void cornellius2Box(SceneData& sceneData, ImageData& imageData, CameraData& cameraData) {}
````

From here you can tinker with the settings and parameters of nearly everything.

To add a new material:
````cpp
auto red = std::make_shared<Lambertian>(Color(0.65f, 0.05f, 0.05f));

auto [materialName] = std::make_shared<[material BSDF]>([constructor arguments, usually color/texture]);
````

You can more on the materials and BSDFs in the materials.h or materials.cpp files.

To add a new object into the scene:
````cpp
world.add(std::make_shared<YZRect>(555.0f, 0.0f, 0.0f, 555.0f, 555.0f, red));

world.add(std::make_shared<[object primitive]>([constructor arguments], [pointer to material created earlier]);
````

With this known you should be able to start tinkering around with the renderer. Depending on your machine you should probably keep the number of samples less than 225 otherwise render times might be too long. Even at 4 samples everything is distinguishable although very noisy. I would recommend 64 but play around.

