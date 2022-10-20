#pragma once
#include <list>
#include <Time.h>
#include <random>
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;
class UniformParticleGenerator :public ParticleGenerator
{
	Vector3 vel_width;
	Vector3 pos_width;

	std::random_device r;
	std::default_random_engine gnd;
	uniform_real_distribution<double>d{-0.5,0.5};
public:
	UniformParticleGenerator(Vector3 pos, Vector3 vel);
	~UniformParticleGenerator();
	void generateParticles(list<Particle*>& l) override;
};