#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator()
{
}

SpringForceGenerator::SpringForceGenerator(Vector3 r, double k, double resting_lenght):r(r),k(k),resting_lenght(resting_lenght) {
}

SpringForceGenerator::SpringForceGenerator(Particle* other, double k, double resting_lenght):
	other(other),k(k),resting_lenght(resting_lenght){
}

void SpringForceGenerator::updateForce(Particle* p, double duration){
	if (fabs(p->getInverseMass()) < 1e-10 || !canUpdateForce(duration)) return;
	Vector3 force = other->getPosition() - p->getPosition();
	const float length = force.normalize();
	const float delta_x = length - resting_lenght;
	force *= delta_x * k;
	p->addForce(force);
}

void SpringForceGenerator::updateForceRigid(PxRigidDynamic* solid, double duration){
	if (fabs(solid->getInvMass()) < 1e-10 || !canUpdateForce(duration)) return;
	Vector3 force = other->getPosition() - solid->getGlobalPose().p;
	const float length = force.normalize();
	const float delta_x = length - resting_lenght;
	force *= delta_x * k;
	solid->addForce(force);
}

SpringForceGenerator::~SpringForceGenerator(){
}
