#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g):gravity(g) {
}

void GravityForceGenerator::updateForce(Particle* p, double t) {
	if (!active||fabs(p->getInverseMass()) < 1e-10 || !canUpdateForce(t)) return;
	else if (canUpdateForce(t))p->addForce(gravity * p->getMass());
}

void GravityForceGenerator::updateForceRigid(PxRigidDynamic* solid, double duration){
	if (!active || fabs(solid->getInvMass()) < 1e-10 || !canUpdateForce(t)) return;
	else if (canUpdateForce(t))solid->addForce(gravity * solid->getMass());
}
