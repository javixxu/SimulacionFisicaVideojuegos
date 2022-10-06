#pragma once
#include <string>
#include <list>
#include "Particle.h"

using namespace std;

class ParticleGenerator{
	std::string name;
public:	
	virtual void generateParticles(list<Particle*>& l)=0;
	void setName(string Name) { name = Name; }
};

