#pragma once
#include "Particle.h"
#include <list>
#include <memory>
#include "ParticleGenerator.h"
using namespace std;
class Firework :public Particle{
	list<shared_ptr<ParticleGenerator>> gens;
	int numGen=0;
public:	enum FIREWORK_TYPE{BASIC=0,CIRCULAR=1,LINEAR=3,NONE=4};
	  FIREWORK_TYPE type;
public:
	Firework();
	~Firework() {
		while (!gens.empty())gens.pop_front();
	}
	Firework(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double size, FIREWORK_TYPE Type_=BASIC,
		Vector4 COLOR = Vector4(1.0, 0.6, 0.2, 1.0), int numGenn=0);
	Firework* clone()const override;
	//void update(double t) override;
	void explode(list<Particle*>&list);
	void addGenerator(shared_ptr<ParticleGenerator> gn) { gens.push_back(gn); }
	void deleteGens() { while (!gens.empty())gens.pop_front(); };
	void setNumGen(int x) { numGen = x; }
	int getNumGen() { return numGen; };
};

