#pragma once
#include "RenderUtils.hpp"
#include <memory>
#include <vector>
#include <string>
#include "core.hpp"

using namespace std;

class Particle{
protected:
	Vector3 vel;
	Vector3 force; 
	Vector3 acc;
	double inverse_mass;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double damping;//rozamiento
	double mass;
	double timeAlive,initialTimeAlive;

	bool isFire_;
	double size_;

	Vector4 color;
public: enum TYPE { UNUSED, PROYECTIL,FIREWORK };
private:TYPE type;
public:
	vector<string>forcesNames;
	//Particle(){};
	Particle(Vector3 Pos=Vector3(0,0,0), Vector3 Vel = Vector3(0, 0, 0), Vector3 acceleration = Vector3(0, 0, 0),
		double Damping = 0.999, double size = 2.0, TYPE Type = TYPE::UNUSED, Vector4 COLOR = Vector4(1.0, 0.6, 0.2, 1.0),double mass=1.0);
	~Particle();
	virtual bool update(double t);
	void integrate(double t);
	bool alive(double t=0.0);
	virtual Particle* clone() const;

	inline void setPosition(Vector3 Pos) {pose.p = Pos;}
	inline void setVelocity(Vector3 Vel) {vel = Vel;}
	inline void setMass(double Mass) {mass = Mass;}
	inline void setAcceleration(Vector3 Acc) {acc = Acc;}
	inline void setDamping(double Damping) {damping = Damping;}
	inline void setTimeAlive(double time) { initialTimeAlive=timeAlive=time;}
	inline void setColor(Vector4 c) { color = c; }
	inline void setIsFire(bool f) { isFire_ = f; }

	inline Vector3 getPosition() { return pose.p; }
	inline Vector3 getVelocity() {return vel; }
	inline double getMass() { return mass; }
	inline Vector3 getAcceleration() { return acc; }
	inline double getDamping() { return damping; }
	inline double getTimeAlive() {return timeAlive; }
	inline double getInitialTimeAlive() { return initialTimeAlive; }
	inline double getInverseMass() { return inverse_mass; }
	void changeSize(double s, physx::PxTransform* pos, Vector4 c);
	inline double getSize() { return size_; };
	inline void setSize(double size) {  size_=size; };
	inline physx::PxTransform* getTransform() {return &pose;}	
	inline Vector4 getColor(){ return color;}
	inline bool isFire() {return isFire_;}
	

	// Clears accumulated force
	void clearForce();
	// Add force to apply in next integration only
	void addForce(const Vector3& f);
};

