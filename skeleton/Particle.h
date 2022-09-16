#pragma once
#include "RenderUtils.hpp"
#include <memory>
#include "core.hpp"
class Particle{
	Vector3 vel;
	physx::PxTransform pose;
	std::unique_ptr<RenderItem> renderItem;
public:
	Particle(Vector3 Pos, Vector3 Vel);
	~Particle();

	void integrate(double t);

};

