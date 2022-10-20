#include "UniformParticleGenenerator.h"

UniformParticleGenerator::UniformParticleGenerator(Vector3 pos, Vector3 vel):vel_width(vel),pos_width(pos) {
	setName("UniformGenerator");
	_model = new Particle();
	_model->setVelocity(Vector3(0.0, 50.0, 0.0));
	_model->setPosition(Vector3(-50.0, 0.0, -150.0));
	_model->setDamping(0.999);
	_model->setTimeAlive(9.0);
	_model->setMass(15.0);
	_model->setAcceleration(Vector3(0.0, 1.0, 0.0));
	gnd = default_random_engine(r());
	num_particles = 5;
}
UniformParticleGenerator::~UniformParticleGenerator() {
	delete _model;
}
void UniformParticleGenerator::generateParticles(list<Particle*>& l) {
	Vector3 newpos, newvel;
	for (int i = 0; i < num_particles; i++) {
		newpos = _model->getPosition() + Vector3(pos_width.x * d(gnd), pos_width.y * d(gnd), pos_width.z * d(gnd));
		newvel = _model->getVelocity() + Vector3(vel_width.x * d(gnd), vel_width.y * d(gnd), vel_width.z * d(gnd));
		Particle* nueva = new Particle(newpos, newvel, _model->getAcceleration(), _model->getDamping(), _model->getMass(), Particle::UNUSED);
		nueva->setTimeAlive(_model->getTimeAlive());
		l.push_back(nueva);
	}
}