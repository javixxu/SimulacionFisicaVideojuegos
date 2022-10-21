#pragma once
#include "ParticleGenerator.h"
#include "Firework.h"
class RocketGenerator :public ParticleGenerator{

public:
	RocketGenerator();
	void generateParticles(list<Particle*>& l) override;
};

