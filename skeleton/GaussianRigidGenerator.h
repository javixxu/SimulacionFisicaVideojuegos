#pragma once
#include "RigidGenerator.h"
#include <Time.h>
#include <random>
class GaussianRigidGenerator :public RigidGenerator{

	double devTip_t;

	std::random_device r;
	default_random_engine gnd;
	normal_distribution<double>d{ 0,1 };

public:
	GaussianRigidGenerator(WorldManager* mngg, typeRigid typee,Vector3	pos, Vector3 vel, double desvTiempo, double masss=0.0, bool rgb=false);
	~GaussianRigidGenerator();
	void generateRigid(list<RigidSolid*>& l) override;
	void increaseDesTipVel(Vector3 increase);
};

