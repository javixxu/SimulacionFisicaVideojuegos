#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g):gravity(g) {
}

void GravityForceGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInverseMass()) < 1e-10) return;
		p->addForce(gravity * p->getMass());
}
