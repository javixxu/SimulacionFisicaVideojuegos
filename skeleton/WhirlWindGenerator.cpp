#include "WhirlWindGenerator.h"

WhirlWindGenerator::WhirlWindGenerator(const Vector3 centre, const Vector3 vel, float K):
WindGenerator(vel,K,K),K(K),centre(centre) {
};
void WhirlWindGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getInverseMass()) < 1e-10 || !canUpdateForce(duration)) return;
	calculateVelocityWind(p->getPosition());
	WindGenerator::updateForce(p, duration);
}
void WhirlWindGenerator::updateForceRigid(PxRigidDynamic* solid, double duration){
	if (fabs(solid->getInvMass()) < 1e-10 || !canUpdateForce(duration)) return;
	calculateVelocityWind(solid->getGlobalPose().p);
	WindGenerator::updateForceRigid(solid, duration);
}
void WhirlWindGenerator::calculateVelocityWind(Vector3 pos) {
	velWind = {-(pos.z-centre.z)-0.157f,50-(pos.y-centre.y),pos.x-centre.x-0.157f};
	velWind *= K;
}