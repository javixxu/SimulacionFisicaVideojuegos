#include "CircleGenerator.h"
#include <math.h>
#include <stdio.h>

#define PI 3.14159265

CircleGenerator::CircleGenerator(int max, int min){
	min_ = min; max_ = max;
	_model = new Particle();
}

void CircleGenerator::generateParticles(list<Particle*>& l) {

	auto random = rand() % max_ + min_;
	double angle = 90;
	for (int i = 0; i < num_particles; i++)
	{
		auto v = Vector3(random*cos(angle * PI / 180.0), 0, random * sin(angle * PI / 180.0));

		Vector3 posFinal = Vector3(_model->getPosition().x, _model->getPosition().y, _model->getPosition().z);
		Vector3 velFinal = Vector3(_model->getVelocity().x + v.x, _model->getVelocity().y + v.y, _model->getVelocity().z + v.z);

		auto p = _model->clone();
		//p->setColor(_model->getColor());
		p->setVelocity(velFinal);
		p->setPosition(posFinal);
		l.push_back(p);
		angle += 360.0 / num_particles;
	}
	changeActive();
}