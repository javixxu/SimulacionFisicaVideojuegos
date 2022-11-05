#pragma once

#include "Particle.h"
#include <list>
#include <random>
class ForceGenerator {
	std::string name;
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	void setName(std::string name_) { name = name_; };
	double t = -1e10;
};