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
		newpos = _model->getPosition() + Vector3(_mean_pos.x * d(gnd), _mean_pos.y * d(gnd), _mean_pos.z * d(gnd));
		newvel = _model->getVelocity() + Vector3(_mean_vel.x * d(gnd), _mean_vel.y * d(gnd), _mean_vel.z * d(gnd));
		Firework* xy;
		switch (type_to_generate) {
		case Firework::BASIC:
			xy = types.at(0)->clone();
		break;
		case Firework::LINEAR:
			xy = types.at(1)->clone();
			break;
		case Firework::CIRCULAR:
			xy = types.at(2)->clone();
		break;
		}
		xy->setPosition(newpos);
		xy->setVelocity(newvel);
		l.push_back(xy);
	}	
	type_to_generate = Firework::BASIC;
}

void RocketGenerator::ChangeType(Firework::FIREWORK_TYPE x){
	 type_to_generate = x; 
}
