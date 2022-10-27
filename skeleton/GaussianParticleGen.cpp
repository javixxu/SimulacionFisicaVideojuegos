#include "GaussianParticleGen.h"
#include <iostream>

GaussianParticleGen::GaussianParticleGen(Vector3 pos, Vector3 vel, double desvTiempo):devTip_pos(pos),devTip_vel(vel),desTip_t(desvTiempo) {
	setName("GaussianGenerator");
	gnd= default_random_engine(r());
	num_particles = 5;
	active = true;
	_model = new Particle();
}

GaussianParticleGen::~GaussianParticleGen() { delete _model; };

void GaussianParticleGen:: generateParticles(list<Particle*>& l) {
	Vector3 newpos, newvel;
	for (int i = 0; i < num_particles; i++){
		newpos= _model->getPosition() + Vector3(devTip_pos.x * d(gnd), devTip_pos.y * d(gnd), devTip_pos.z * d(gnd));
		newvel= _model->getVelocity() + Vector3(devTip_vel.x * d(gnd), devTip_vel.y * d(gnd), devTip_vel.z * d(gnd));
		
		Particle* nueva = _model->clone(); 
		nueva->setPosition(newpos);
		nueva->setVelocity(newvel);
		nueva->setTimeAlive(_model->getInitialTimeAlive()+desTip_t*d(gnd));
		nueva->setIsFire(_model->isFire());
		l.push_back(nueva);
	}
}

void GaussianParticleGen::increaseDesTipVel(Vector3 increase)
{
	devTip_vel += increase;
	if (devTip_vel.x < 0) devTip_vel.x = 0;
	if (devTip_vel.y < 0) devTip_vel.y = 0;
	if (devTip_vel.z < 0) devTip_vel.z = 0;
}
