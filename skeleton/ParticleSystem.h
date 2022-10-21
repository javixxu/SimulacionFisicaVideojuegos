#pragma once
#include <list>
#include <Time.h>

#include "Particle.h"
#include "ParticleGenerator.h"
#include "Firework.h"

using namespace std;
class ParticleSystem{

	list<Particle*> list_particles;
	list<shared_ptr<ParticleGenerator>>list_generator;
	vector<Firework*> fireworks_pool;
public:
	ParticleSystem();
	~ParticleSystem();
	void update(double t);
	shared_ptr<ParticleGenerator> getParticleGenerator(string name);

	void generateFireworkSystem();//no creado
	void shootFirework(Firework::FIREWORK_TYPE type);
	void onParticleDeath(Particle* pt);
	
};

