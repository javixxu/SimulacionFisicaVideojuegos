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
		molde.setVelocity(Vector3(30.0, 50.0, -6.0));
		molde.setPosition(Vector3(-200.0, 0.0, -150.0));
		molde.setDamping(0.999);
		molde.setTimeAlive(10.0);
		molde.setMass(15.0);
		molde.setAcceleration(Vector3(0.0, -10.0, 0.0));
	}
public:
	GaussianParticleGen(Vector3	pos,Vector3 vel);
	~GaussianParticleGen();
	void generateParticles(list<Particle*>& l) override;
};