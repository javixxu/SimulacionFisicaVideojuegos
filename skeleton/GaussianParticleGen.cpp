#include "GaussianParticleGen.h"
#include <iostream>

GaussianParticleGen::GaussianParticleGen(Vector3 pos, Vector3 vel, double desvTiempo,double massi,bool rgb):devTip_pos(pos),devTip_vel(vel),desTip_t(desvTiempo) {
	setName("GaussianGenerator");
	gnd= default_random_engine(r());
	num_particles = 5;
	active = true;
	_model = new Particle();
	mass = massi; rgbRandom = rgb;
}

GaussianParticleGen::~GaussianParticleGen() { delete _model; };

void GaussianParticleGen:: generateParticles(list<Particle*>& l) {
	Vector3 newpos, newvel;
	for (int i = 0; i < num_particles; i++){
		newpos= _model->getPosition() + Vector3(devTip_pos.x * d(gnd), devTip_pos.y * d(gnd), devTip_pos.z * d(gnd));
		newvel= _model->getVelocity() + Vector3(devTip_vel.x * d(gnd), devTip_vel.y * d(gnd), devTip_vel.z * d(gnd));
		auto lastMass = _model->getMass();
		_model->setMass(_model->getMass() + abs(d(gnd)*mass));
		if (name=="ExplosionSystem") {		
			auto x = _model->getMass();
			if(x>=3.0)_model->setColor(Vector4(1.0,0.0,0.0,1.0));
			else if(x>=2.0)_model->setColor(Vector4(1.0, 1.0, 0.0, 1.0));
			else _model->setColor(Vector4(0.0, 1.0, 0.0, 1.0));
		}
		else if (rgbRandom) {
			auto r = rand() % 255 + 0;
			auto g = rand() % 255 + 0;
			auto b = rand() % 255 + 0;
			_model->setColor(Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0));
		}
		Particle* nueva = _model->clone();
		_model->setMass(lastMass);
		nueva->setPosition(newpos);
		nueva->setVelocity(newvel);
		nueva->setTimeAlive(_model->getInitialTimeAlive()+desTip_t*d(gnd));
		nueva->setIsFire(_model->isFire());
		for (int i = 0; i < _model->forcesNames.size(); i++)
			pfr->addRegistry(pfr->getForceGenenarion(_model->forcesNames[i]), nueva);
		
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
