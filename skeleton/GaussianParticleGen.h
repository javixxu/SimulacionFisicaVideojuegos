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
	double desTip_t;

	std::random_device r;
	default_random_engine gnd;
	normal_distribution<double>d{ 0,1 };


public:
	GaussianParticleGen(Vector3	pos,Vector3 vel,double desvTiempo, double mass=0, bool rgb=false);
	~GaussianParticleGen();
	void generateParticles(list<Particle*>& l) override;
	void increaseDesTipVel(Vector3 increase);
};