#include "Integrator.h"

long long int integrationCount = 0;


// The scene is a BVH_Node pointer to the head of the tree
Color UDPTIntegrator::Li(const Ray& rayIn, std::shared_ptr<Shape> scene, const Color& backgroundColor, int depth) const {

	// Russian Roulette Probability
	float rrProb = 1.0f;
	if (depth >= this->minDepth) {
		rrProb = this->rrProbability; // 100%
	}

	Interaction rec;
	if (!scene->hit(rayIn, 0.0000001f, EQMath::INF, 0.0f, rec)) {
		return backgroundColor;
	}

	Color emit = rec.material->Le(rec.p, rec.uv); // Black bodies (lights) will have this return Color(R > 0, G > 0, B > 0)

	// Russian Roulette Missed (Path Ends)
	if (EQMath::random(0.001f, 1.0f) >= rrProb) {
		return emit;
	}

	Vector3f omega(0.0f, 0.0f, 0.0f); // Omega is in local coordinates
	Vector3f woWorld(0.0f, 0.0f, 0.0f);
	Vector3f wiWorld(0.0f, 0.0f, 0.0f);
	Color bsdf;
	float pdf;
	Ray rayOut;


	// std::cerr << rec.material->type << std::endl;
	switch (rec.material->getType()) {
	case DIFFUSE:
	{
		omega = rec.material->sampleBSDF(rec.p, rec.sNormal); // For Diffuse because random hemisphere scattering, no arguments needed
		woWorld = omega.x() * rec.uvw.U() + omega.y() * rec.uvw.V() + omega.z() * rec.uvw.W(); // Convert to world coordinates using ONB
		// woWorld = omega;
		// wiWorld = rayIn.direction().x() * rec.uvw.U() + rayIn.direction().y() * rec.uvw.V() + rayIn.direction().z() * rec.uvw.W();
		wiWorld = rayIn.direction();
	}
	case BLACKBODY:
	{
		omega = rec.material->sampleBSDF(rec.p, rec.sNormal); // For Diffuse because random hemisphere scattering, no arguments needed
		woWorld = omega.x() * rec.uvw.U() + omega.y() * rec.uvw.V() + omega.z() * rec.uvw.W(); // Convert to world coordinates using ONB
		// woWorld = omega;
		// wiWorld = rayIn.direction().x() * rec.uvw.U() + rayIn.direction().y() * rec.uvw.V() + rayIn.direction().z() * rec.uvw.W();
		wiWorld = rayIn.direction();
	}
	case SPECULAR:
	{
		omega = rec.material->sampleBSDF(rayIn.direction(), rec.sNormal);
		woWorld = omega; // Convert to world coordinates using ONB
		// woWorld = omega.x() * rec.uvw.U() + omega.y() * rec.uvw.V() + omega.z() * rec.uvw.W();
		// wiWorld = rayIn.direction().x() * rec.uvw.U() + rayIn.direction().y() * rec.uvw.V() + rayIn.direction().z() * rec.uvw.W();
		wiWorld = rayIn.direction();
	}
	}

	bsdf = rec.material->f(wiWorld, woWorld, rec);
	pdf = rec.material->PDF(wiWorld, woWorld, rec);
	rayOut = Ray(rec.p, woWorld, rayIn.time(), rec.material->ior);

	// If the bsdf is already 0, then no point in recursing further because light does not reflect off this material
	
	++integrationCount;
	

	return emit + bsdf * this->Li(rayOut, scene, backgroundColor, depth + 1) / pdf;
}


/*
// Light Source Sampling Direct Illunimation
Color UDPTIntegrator::Li(const Ray& rayIn, std::shared_ptr<Shape> scene, const Color& backgroundColor, int depth) const {

}
*/
