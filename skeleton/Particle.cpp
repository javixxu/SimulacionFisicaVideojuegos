#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel):pose(Pos),vel(Vel),acc(Vector3(0)) {
																//tipo de geometria    posicion   tamaño
	renderItem = std::make_unique<RenderItem>(CreateShape(physx::PxSphereGeometry(2)), &pose, Vector4(1.0, 0.6, 0.2, 1.0));
}

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 acceleration) :pose(Pos), vel(Vel),acc(acceleration){
	renderItem = std::make_unique<RenderItem>(CreateShape(physx::PxSphereGeometry(2)), &pose, Vector4(1.0, 0.6, 0.2, 1.0));
}

Particle::~Particle(){
	renderItem.release(); renderItem = nullptr;
}

void Particle::integrate(double t){
	pose.p += vel * t;
	vel += acc*t;
	vel *= powf(damping, t);
}
