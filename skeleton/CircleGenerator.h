#pragma once
#include "ParticleGenerator.h"
#include <random>

using namespace std;

class CircleGenerator : public ParticleGenerator
{
	int max_, min_;
public:
	CircleGenerator(int max, int min);
	~CircleGenerator() = default;

	void generateParticles(list<Particle*>& l) override;

};