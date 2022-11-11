#pragma once

#include "Particle.h"
#include <list>
#include <string>
#include <random>
class ForceGenerator {
	std::string name;
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	void setName(std::string name_) { name = name_; };
	string getName() {
		return name;
	}
	double t = -1e10;
};