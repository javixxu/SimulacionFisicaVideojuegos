#include "ExplosionForceGenerator.h"
#include "Particle.h"
ExplosionForceGenerator::ExplosionForceGenerator(double R, double K, Vector3 centre,double time):centre(centre),R(R),K(K),time(time) {
	active = false;
}
void ExplosionForceGenerator::updateForce(Particle* p, double duration){
	if (!canUpdateForce(duration) || fabs(p->getInverseMass()) < t)return;
	timeActive += duration;
	if (timeActive <= time)R=velSonido*timeActive;
	float r = sqrt(pow(p->getPosition().x - centre.x, 2) + pow((p->getPosition().y - centre.y), 2) + pow((p->getPosition().z - centre.z), 2));
	Vector3 force={0,0,0};
	if (r < R){
		double intens = K / pow(r, 2);
		double intens2 = exp(-(duration / time));
		force= {(p->getPosition().x - centre.x), (p->getPosition().y - centre.y), (p->getPosition().z - centre.z)};
		force *= intens * intens2;
	}	 
	p->addForce(force);
}

void ExplosionForceGenerator::updateForceRigid(PxRigidDynamic* solid, double duration){
	if (!canUpdateForce(duration) || fabs(solid->getInvMass()) < t)return;
	timeActive += duration;
	if (timeActive <= time)R = velSonido * timeActive;
	float r = sqrt(pow(solid->getGlobalPose().p.x - centre.x, 2) + pow((solid->getGlobalPose().p.y - centre.y), 2) + pow((solid->getGlobalPose().p.z - centre.z), 2));
	Vector3 force = { 0,0,0 };
	if (r < R) {
		double intens = K / pow(r, 2);
		double intens2 = exp(-(duration / time));
		force = { (solid->getGlobalPose().p.x - centre.x), (solid->getGlobalPose().p.y - centre.y), (solid->getGlobalPose().p.z - centre.z) };
		force *= intens * intens2;
	}
	solid->addForce(force);
}
