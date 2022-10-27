#pragma once
#include "ParticleGenerator.h"
#include "Firework.h"
#include <random>

class RocketGenerator :public ParticleGenerator{

	vector<Firework*> types;

	std::random_device r;
	std::default_random_engine gnd;
	uniform_real_distribution<double>d{ -0.5,0.5 };
	
public:
	RocketGenerator(Vector3 pos, Vector3 vel);
	void generateParticles(list<Particle*>& l) override;
	void setTypesRockets(vector<Firework*> f) { types = f; };
};

