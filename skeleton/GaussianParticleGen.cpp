#include "GaussianParticleGen.h"

GaussianParticleGen::GaussianParticleGen(Vector3 pos, Vector3 vel):devTip_pos(pos),devTip_vel(vel) {
	setName("GaussianGenerator");
	generaMolde();
}

GaussianParticleGen::~GaussianParticleGen() {

}
list<Particle*> GaussianParticleGen::generateParticles() {
	list<Particle*> l; Vector3 newpos, newvel; default_random_engine gnd;
	for (int i = 0; i < 3; i++){
		newpos= molde.getPosition() + Vector3(devTip_pos.x * d(gnd), devTip_pos.y * d(gnd), devTip_pos.z * d(gnd));
		newvel= molde.getVelocity() + Vector3(devTip_vel.x * d(gnd), devTip_vel.y * d(gnd), devTip_vel.z * d(gnd));
		Particle* nueva = new Particle(newpos,newvel,molde.getAcceleration(),molde.getDamping(),molde.getMass(),Particle::UNUSED);
		l.push_back(nueva);
	}
	return l;
}