#pragma once
#include "RenderUtils.hpp"
#include <memory>
#include "core.hpp"

class Particle{
	Vector3 vel;
	Vector3 acc;
	physx::PxTransform pose;
	std::unique_ptr<RenderItem> renderItem;
	double damping;//rozamiento
	double mass;
	
public: enum TYPE { UNUSED, PROYECTIL };
private:TYPE type;
public:
	Particle(Vector3 Pos, Vector3 Vel,TYPE Type);
	Particle(Vector3 Pos, Vector3 Vel, Vector4 COLOR, TYPE Type);
	Particle(Vector3 Pos, Vector3 Vel,Vector3 acceleration, double Damping,double Mass, TYPE Type);
	~Particle();
	void setVelocity(Vector3 Vel) {
		vel = Vel;
	}
	inline void setMass(double Mass) {
		mass = Mass;
	}
	inline void setAcceleration(Vector3 Acc) {
		acc = Acc;
	}
	inline void setDamping(double Damping) {
		damping = Damping;
	}
	void integrate(double t);

};

