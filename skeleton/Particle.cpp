#include "Particle.h"


Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping,double size,TYPE Type, Vector4 COLOR) :
	pose(Pos), vel(Vel),acc(acceleration),damping(Damping),size_(size),type(Type),color(COLOR), isFire_(false) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size_)), &pose,color);
}

Particle::~Particle(){
	//renderItem.release(); renderItem = nullptr;
	DeregisterRenderItem(renderItem);
}

bool Particle::update(double t){
	if (alive(t)) { integrate(t); return true; };
	return false;
}

void Particle::integrate(double t){
	pose.p += vel * t;
	vel += acc*t;
	vel *= powf(damping, t);
}
Particle* Particle::clone() const
{
	Particle* ptr = new Particle(pose.p, vel, acc,
		damping, size_, type, color);
	ptr->setTimeAlive(initialTimeAlive);
	ptr->setIsFire(isFire_);
	ptr->setColor(color);
	return ptr;
}
void Particle::changeSize(double s, physx::PxTransform* pos, Vector4 c)
{
	DeregisterRenderItem(renderItem);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(s)), pos, c);
}
;
bool Particle::alive(double t) {
	timeAlive -= t;
	return timeAlive>0;
}