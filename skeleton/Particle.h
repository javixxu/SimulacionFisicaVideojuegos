#pragma once
#include "RenderUtils.hpp"
#include <memory>
#include "core.hpp"

class Particle{
protected:
	Vector3 vel;
	Vector3 acc;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double damping;//rozamiento
	double mass;
	double timeAlive,initialTimeAlive;

	Vector4 color;
public: enum TYPE { UNUSED, PROYECTIL,FIREWORK };
private:TYPE type;
public:
	Particle():type(TYPE::UNUSED),damping(0.0),acc(Vector3()),vel(Vector3()) {};
	Particle(Vector3 Pos, Vector3 Vel, TYPE Type, Vector4 COLOR = Vector4(1.0, 0.6, 0.2, 1.0));
	Particle(Vector3 Pos, Vector3 Vel, Vector4 COLOR, TYPE Type);
	Particle(Vector3 Pos, Vector3 Vel,Vector3 acceleration, double Damping,double Mass, TYPE Type, Vector4 COLOR = Vector4(1.0, 0.6, 0.2, 1.0));
	~Particle();
	virtual void update(double t) { integrate(t); alive(t); };
	void integrate(double t);
	bool alive(double t);
	virtual Particle* clone() const;

	inline void setPosition(Vector3 Pos) {pose.p = Pos;}
	inline void setVelocity(Vector3 Vel) {vel = Vel;}
	inline void setMass(double Mass) {mass = Mass;}
	inline void setAcceleration(Vector3 Acc) {acc = Acc;}
	inline void setDamping(double Damping) {damping = Damping;}
	inline void setTimeAlive(double time) { initialTimeAlive=timeAlive=time;}
	inline double getInitialTimeAlive() { return initialTimeAlive; }

	inline Vector3 getPosition() { return pose.p; }
	inline Vector3 getVelocity() {return vel; }
	inline double getMass() { return mass; }
	inline Vector3 getAcceleration() { return acc; }
	inline double getDamping() { return damping; }
	inline double getTimeAlive() {return timeAlive; }
	inline void setColor(Vector4 c) {
		color = c;
	}
};

