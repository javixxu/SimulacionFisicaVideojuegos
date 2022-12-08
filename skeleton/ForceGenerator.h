#pragma once

#include "Particle.h"
#include <list>
#include <string>
#include <random>
#include <iostream>

using namespace std;
using namespace physx;

class ForceGenerator {
protected:
	std::string name;
	double timerLimit=-1;
	double timer=0.0;
	bool active = true;
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	virtual void updateForceRigid(PxRigidDynamic* solid, double duration) = 0;
	void setName(std::string name_) { name = name_;};
	string getName() {
		return name;
	}
	void setTimeLimit(double x) { timerLimit = x; };
	void clearTimer() { timer = 0.0; };
	void setActive(bool x, double timeLimitt = -1) {
		active = x; timerLimit = timeLimitt; clearTimer();};
	bool canUpdateForce(double duration) {
		if (!active) return false;
		else if (timerLimit == -1)return true;		

		bool x =  timerLimit >= timer + duration;
		timer += duration;
		active=x;
		if (!x)cout << "OFF" << "\n";
		return x;
		return true;
	};
	double t = -1e10;
};