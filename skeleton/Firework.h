#pragma once
#include "Particle.h"
#include <list>
#include <memory>
using namespace std;
class Firework :public Particle{
	list<shared_ptr<Particle>> _gens;
public:
	Firework();
	Firework(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double Mass, TYPE Type, Vector4 COLOR);
	Firework* clone()const override;
	int update(double t);
	void explode(list<Particle*>list);
};

