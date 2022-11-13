#include "WhirlWindGenerator.h"

WhirlWindGenerator::WhirlWindGenerator(const Vector3 centre, const Vector3 vel, float K):
WindGenerator(vel,K,K),K(K),centre(centre) {
};
void WhirlWindGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getInverseMass()) < 1e-10) return;
	calculateVelocityWind(p->getPosition());
	WindGenerator::updateForce(p, duration);
};
void WhirlWindGenerator::calculateVelocityWind(Vector3 pos) {
	velWind = {-(pos.z-velWind.z),50-(pos.y-velWind.y),pos.x-velWind.x};
	velWind *= K;
}