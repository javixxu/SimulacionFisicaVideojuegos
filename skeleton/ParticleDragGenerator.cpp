#include "ParticleDragGenerator.h"

ParticleDragGenerator::ParticleDragGenerator(const float k1_, const float k2_):k1(k1_),k2(k2_) {	
}

void ParticleDragGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInverseMass()) < 1e-10) return;
	p->addForce(calculateDrag(p->getVelocity()));
}
Vector3 ParticleDragGenerator::calculateDrag(Vector3 v) {
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = k1 * drag_coef + k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;
	return dragF;
}