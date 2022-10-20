#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel,TYPE Type=UNUSED):pose(Pos),vel(Vel),acc(Vector3(0)),damping(0.999),mass(0.0),type(UNUSED) {
																//tipo de geometria    posicion   tamaño
	renderItem = new RenderItem( CreateShape(physx::PxSphereGeometry(2)), &pose, Vector4(1.0, 0.6, 0.2, 1.0));
}

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector4 COLOR, TYPE Type = TYPE::UNUSED)
	:pose(Pos), vel(Vel), acc(Vector3(0)), damping(1.0), mass(0.0), type(Type) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(2)), &pose, COLOR);
}
Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping=0.999,double Mass=0.0,TYPE Type=TYPE::UNUSED, Vector4 COLOR= Vector4(1.0, 0.6, 0.2, 1.0)) :
	pose(Pos), vel(Vel),acc(acceleration),damping(Damping),mass(Mass),type(Type) {
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(2)), &pose,COLOR);
}

Particle::~Particle(){
	//renderItem.release(); renderItem = nullptr;
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t){
	pose.p += vel * t;
	vel += acc*t;
	vel *= powf(damping, t);
}
Particle* Particle::clone() const
{
	Particle* ptr = new Particle(pose.p, vel, acc,
		damping, mass, type);
	ptr->timeAlive = timeAlive;
	return ptr;
};
bool Particle::alive(double t) {
	timeAlive -= t;
	return timeAlive>0;
}