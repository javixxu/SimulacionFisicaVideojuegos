#pragma once
#include "WindGenerator.h"

class WhirlWindGenerator :public WindGenerator {
	Vector3 centre;
	float K;
public:
	WhirlWindGenerator(const Vector3 centre = { 0,0,0 }, const Vector3 vel = { 0,0,0 },float K=0.0);
	void updateForce(Particle* p, double duration) override;
	void updateForceRigid(PxRigidDynamic* solid, double duration) override;
	void calculateVelocityWind(Vector3 pos);
	
};