#pragma once
#include "Particle.h"
#include <list>
#include <memory>
#include "ParticleGenerator.h"
using namespace std;
class Firework :public Particle{
	list<shared_ptr<ParticleGenerator>> gens;
public:	enum FIREWORK_TYPE{BASIC};
	  FIREWORK_TYPE type;
public:
	Firework();
	Firework(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double Mass, FIREWORK_TYPE Type_, Vector4 COLOR = Vector4(1.0, 0.6, 0.2, 1.0));
	Firework* clone()const override;
	void update(double t) override;
	void explode(list<Particle*>&list);
	void addGenerator(shared_ptr<ParticleGenerator> gn) { gens.push_back(gn); }
};

