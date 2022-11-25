#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g):gravity(g) {
}

void GravityForceGenerator::updateForce(Particle* p, double t) {
	if (!active||fabs(p->getInverseMass()) < 1e-10) return;
	else if (canUpdateForce(t))p->addForce(gravity * p->getMass());
}
