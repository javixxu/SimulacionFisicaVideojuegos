#pragma once
#include <list>
#include <Time.h>
#include <random>
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;
class GaussianParticleGen:public ParticleGenerator
{
	Vector3 devTip_vel;
	Vector3 devTip_pos;
	Particle molde;
	normal_distribution<double>d{ 0,1 };
	void generaMolde() {
		molde.setVelocity(Vector3(20.0, 20.0, 0.0));
		molde.setPosition(Vector3(15.0, 15.0, 0.0));
		molde.setDamping(0.0);
		molde.setTimeAlive(10.0);
		molde.setAcceleration(Vector3(0.1, -2.8, 0.0));
	}
public:
	GaussianParticleGen(Vector3	pos,Vector3 vel);
	~GaussianParticleGen();
	list<Particle*> generateParticles() override;
};