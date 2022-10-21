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
	bool active;
public:	
	virtual void generateParticles(list<Particle*>& l)=0;
	void setName(string Name) { name = Name; }
	string getName() { return name; }
	void setParticle(Particle* _Model) {
		_model = _Model;
	}
	inline bool isActive() { return active; }
	void setActive(bool x) { active = x; }
	void setNumGenerator(int x) { num_particles = x; };
};

