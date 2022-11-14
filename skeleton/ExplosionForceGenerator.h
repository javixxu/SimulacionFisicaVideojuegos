#pragma once
#include "ForceGenerator.h"
#include <cmath>
#include <iostream>
class ExplosionForceGenerator:public ForceGenerator {
	double R;
	double K;
	double time;
	Vector3 centre;
	bool active;
public:
	ExplosionForceGenerator(double R,double K,Vector3 centre,double time);
	virtual void updateForce(Particle* p, double duration);
	void OnActive(bool activee) { active = activee; };
};