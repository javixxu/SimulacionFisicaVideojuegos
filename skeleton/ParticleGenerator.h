#pragma once
#include <string>
#include <list>
#include "Particle.h"

using namespace std;

class ParticleGenerator{
protected:
	std::string name;
	Vector3 _mean_pos, _mean_vel;
	int num_particles;
	Particle* _model;
public:	
	virtual void generateParticles(list<Particle*>& l)=0;
	void setName(string Name) { name = Name; }
	string getName() { return name; }
	void setParticle(Particle* _Model) {
		_model = _Model;
	}
};

