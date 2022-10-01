#pragma once
#include <string>
#include <list>
#include "Particle.h"

using namespace std;

class ParticleGenerator{
	std::string name;
public:	
	virtual list<Particle*>generateParticles()=0;
	void setName(string Name) { name = Name; }
};

