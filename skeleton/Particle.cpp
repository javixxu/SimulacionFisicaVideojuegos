#include "Particle.h"


Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping,double size,TYPE Type, Vector4 COLOR, double mass) :
	pose(Pos), vel(Vel),acc(acceleration),damping(Damping),size_(size),type(Type),color(COLOR), isFire_(false),mass(mass),inverse_mass(1/mass) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size_)), &pose, color); force = { 0,0,0 };
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
	// Trivial case, infinite mass --> do nothing
	if (inverse_mass<= 0.0f) return;
	//update position
	pose.p += vel * t;
	Vector3 totalAcceleration = acc;
	totalAcceleration += force * mass/*inverse_mass*/;
	//Update linear velocity
	vel += totalAcceleration *t;
	//Impose drag (damping)
	vel *= powf(damping, t);

	clearForce();
}
Particle* Particle::clone() const
{
	Particle* ptr = new Particle(pose.p, vel, acc,
		damping, size_, type, color,mass);
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

bool Particle::alive(double t) {
	timeAlive -= t;
	return timeAlive>0;
}

void Particle::clearForce() { 
	force=Vector3(0);
}
void Particle::addForce(const Vector3& f) { 
	force += f; 
}