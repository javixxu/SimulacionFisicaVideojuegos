#pragma once
#include "RenderUtils.hpp"
#include <memory>
#include "core.hpp"
class Particle{
	Vector3 vel;
	Vector3 acc;
	physx::PxTransform pose;
	std::unique_ptr<RenderItem> renderItem;
	double damping = 0.999;//rozamiento
public:
	Particle(Vector3 Pos, Vector3 Vel);
	Particle(Vector3 Pos, Vector3 Vel,Vector3 acceleration);
	~Particle();

	void integrate(double t);

};

