#include "RocketGenerator.h"

RocketGenerator::RocketGenerator(Vector3 pos, Vector3 vel) {

	active = true;
	gnd = default_random_engine(r());
	num_particles = 1;
	_mean_pos = pos;
	_mean_vel = vel;
	_model = new Particle();
}

void RocketGenerator::generateParticles(list<Particle*>& l){
	Vector3 newpos, newvel;
	for (int i = 0; i < num_particles; i++){
		int random = rand() % 3;
		newpos = _model->getPosition() + Vector3(_mean_pos.x * d(gnd), _mean_pos.y * d(gnd), _mean_pos.z * d(gnd));
		newvel = _model->getVelocity() + Vector3(_mean_vel.x * d(gnd), _mean_vel.y * d(gnd), _mean_vel.z * d(gnd));
		Firework* xy;
		switch (random) {
		case 0:/*Firework::BASIC:*/
			xy = types.at(0)->clone();
		break;
		case 1: /*Firework::LINEAR:*/
			xy = types.at(1)->clone();
			break;
		case 2: /*Firework::CIRCULAR:*/
			xy = types.at(2)->clone();
		break;
		}
		xy->setPosition(newpos);
		xy->setVelocity(newvel);
		l.push_back(xy);
	}
	changeActive();
}