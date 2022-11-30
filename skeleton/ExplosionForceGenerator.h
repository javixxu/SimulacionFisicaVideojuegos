#pragma once
#include "ForceGenerator.h"
#include <cmath>
#include <iostream>
class ExplosionForceGenerator:public ForceGenerator {
	double R;
	double K;
	double time;
	double timeActive;
	Vector3 centre;
	const double velSonido= 343;
public:
	ExplosionForceGenerator(double R,double K,Vector3 centre,double time);
	virtual void updateForce(Particle* p, double duration);
	void OnActive(bool activee) { setActive(activee); timeActive = 0; };
};