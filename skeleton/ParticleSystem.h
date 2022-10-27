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
	Vector3 gravity = Vector3(0.0, -9.8, 0.0);

public:
	ParticleSystem();
	~ParticleSystem();
	void update(double t);
	shared_ptr<ParticleGenerator> getParticleGenerator(string name);

	void generateFireworkSystem();//no creado
	void generateFogSystem();
	void generateFlamesSystem();
	void generateHosepipeSystem();
	void generateCircleSystem();
	void generateRocketSystem();
	void increaseDesTip(Vector3 increase);

	void shootFirework(Firework::FIREWORK_TYPE type);
	void onParticleDeath(Particle* pt);
	
};

