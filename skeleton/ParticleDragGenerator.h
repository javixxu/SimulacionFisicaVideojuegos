#pragma once
#include "ForceGenerator.h"
#include <iostream>
#include "core.hpp"

using namespace std;

class ParticleDragGenerator :public ForceGenerator {
public:
	ParticleDragGenerator();
	ParticleDragGenerator(const float k1, const float k2);
	virtual void updateForce(Particle* p, double duration);
	inline void setDrag(float k1_, float k2_) { k1 = k1_; k2 = k2_; }
	inline float getK1() { return k1; }
	inline float getK2() { return k2; }
	Vector3 calculateDrag(Vector3 vel);
protected:
	float k1;
	float k2;
};