#include "WindGenerator.h"
WindGenerator::WindGenerator(const Vector3 vel, const float k1, const float k2, Vector3 centre,float range):
	ParticleDragGenerator(k1,k2),velWind(vel),out(centre) ,range(range) {
};
void WindGenerator::updateForce(Particle* p, double duration){
	if (fabs(p->getInverseMass()) < 1e-10 || !aplicateWindForce(p->getPosition()) || !canUpdateForce(duration)) return; 
	
	p->addForce(calculateDrag(p->getVelocity() - velWind));		
}
void WindGenerator::updateForceRigid(PxRigidDynamic* solid, double duration){
	if (solid->getInvMass() < 1e-10 || !aplicateWindForce(solid->getGlobalPose().p) || !canUpdateForce(duration))return;

	solid->addForce(calculateDrag(solid->getLinearVelocity() - velWind));
}
bool WindGenerator::aplicateWindForce(Vector3 pos){
	return range==-1||
		(pos.x >= out.x-range && pos.x <= out.x+range) &&
		(pos.y >= out.y-range && pos.y <= out.y+range) &&
		(pos.z >= out.z-range && pos.z <= out.z+range);
}
