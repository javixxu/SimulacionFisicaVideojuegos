#pragma once
#include <list>
#include <Time.h>

#include "Particle.h"
#include "ParticleGenerator.h"

using namespace std;
class ParticleSystem{

	list<Particle*> list_particles;
	list<ParticleGenerator*>list_generator;
	Vector3 max_pos;
	Vector3 min_pos;
	Vector3 gravity;
public:
	ParticleSystem();
	~ParticleSystem();
	void update(double t);
};

