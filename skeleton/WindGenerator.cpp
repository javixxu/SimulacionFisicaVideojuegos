#include "WindGenerator.h"
WindGenerator::WindGenerator(const Vector3 vel, const float k1, const float k2):ParticleDragGenerator(k1,k2),velWind(vel) {
};
void WindGenerator::updateForce(Particle* p, double duration){
	if (fabs(p->getInverseMass()) < 1e-10) return;
	p->addForce(calculateDrag(p->getVelocity() - velWind));
};