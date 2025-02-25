#include "UniformParticleGenenerator.h"

UniformParticleGenerator::UniformParticleGenerator(Vector3 pos, Vector3 vel, double massi , bool rgb):
	vel_width(vel),pos_width(pos) {
	setName("UniformGenerator");
	_model = new Particle();
	rgbRandom = rgb; mass = massi;
	gnd = default_random_engine(r());
	num_particles = 5;
	active = true;
}
UniformParticleGenerator::~UniformParticleGenerator() {
	 _model->~Particle();
}
void UniformParticleGenerator::generateParticles(list<Particle*>& l) {
	Vector3 newpos, newvel;
	for (int i = 0; i < num_particles; i++) {
		newpos = _model->getPosition() + Vector3(pos_width.x * d(gnd), pos_width.y * d(gnd), pos_width.z * d(gnd));
		newvel = _model->getVelocity() + Vector3(vel_width.x * d(gnd), vel_width.y * d(gnd), vel_width.z * d(gnd));
		if (rgbRandom) {
			auto r = rand() % 255 + 0;
			auto g = rand() % 255 + 0;
			auto b = rand() % 255 + 0;
			_model->setColor(Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0));
		}
		auto lastMass = _model->getMass();
		_model->setMass(_model->getMass() + abs(d(gnd) * mass));
		Particle* nueva = _model->clone(); 
		_model->setMass(lastMass);

		nueva->setPosition(newpos); nueva->setVelocity(newvel);
		nueva->setTimeAlive(_model->getInitialTimeAlive());
		l.push_back(nueva);
	}
}