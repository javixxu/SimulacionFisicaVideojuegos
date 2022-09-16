#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel):pose(Pos),vel(Vel) {
	
	renderItem = std::make_unique<RenderItem>(CreateShape(physx::PxSphereGeometry(2)), &pose, Vector4(1.0, 0.6, 0.2, 1.0));
}

Particle::~Particle(){
	renderItem.release(); renderItem = nullptr;
}

void Particle::integrate(double t){
	pose.p += vel * t;
}
