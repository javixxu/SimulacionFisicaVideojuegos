#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class SpringForceGenerator :public ForceGenerator{
protected:
	double k;
	double resting_lenght;
	double time;
	Vector3 r;
	Particle* other;
public:
	SpringForceGenerator();
	SpringForceGenerator(Vector3 r, double k, double resting_lenght);
	SpringForceGenerator(Particle* other, double k, double resting_lenght);
	virtual void updateForce(Particle* p, double duration) override;
	~SpringForceGenerator();
	inline void setK(double kk) { k = kk; };
	void increaseK(double kk) { k += kk; };
};

