#include "GaussianParticleGen.h"

GaussianParticleGen::GaussianParticleGen(Vector3 pos, Vector3 vel, double desvTiempo):devTip_pos(pos),devTip_vel(vel),desTip_t(desvTiempo) {
	setName("GaussianGenerator");
	_model = new Particle();
	_model->setVelocity(Vector3(30.0, 50.0, -6.0));
	_model->setPosition(Vector3(-200.0, 0.0, -150.0));
	_model->setDamping(0.999);
	_model->setTimeAlive(9.0);
	_model->setMass(15.0);
	_model->setAcceleration(Vector3(0.0, -10.0, 0.0));
	gnd= default_random_engine(r());
	num_particles = 5;
}

GaussianParticleGen::~GaussianParticleGen() { delete _model; };
void GaussianParticleGen:: generateParticles(list<Particle*>& l) {
	Vector3 newpos, newvel;
	for (int i = 0; i < num_particles; i++){
		newpos= _model->getPosition() + Vector3(devTip_pos.x * d(gnd), devTip_pos.y * d(gnd), devTip_pos.z * d(gnd));
		newvel= _model->getVelocity() + Vector3(devTip_vel.x * d(gnd), devTip_vel.y * d(gnd), devTip_vel.z * d(gnd));
		Particle* nueva = new Particle(newpos,newvel, _model->getAcceleration(), _model->getDamping(), _model->getMass(),Particle::UNUSED);
		nueva->setTimeAlive(_model->getTimeAlive()+desTip_t*d(gnd));
		l.push_back(nueva);
	}
}