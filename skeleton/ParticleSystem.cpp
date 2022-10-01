#include "ParticleSystem.h"
#include "GaussianParticleGen.h"
ParticleSystem::ParticleSystem() {
	list_generator.push_back(new GaussianParticleGen(Vector3(10.0,10.0,0.0),Vector3(15.0,10.0,0.0)));
	list_particles = list<Particle*>();
}
ParticleSystem::~ParticleSystem() {
	for (auto i = list_generator.begin(); i != list_generator.end(); i++){
		delete* i;
		list_generator.erase(i);
	}
	for (auto i = list_particles.begin(); i != list_particles.end(); i++) {
		delete* i;
		list_particles.erase(i);
	}
	
}

void ParticleSystem::update(double t) {
	for (auto i = list_generator.begin(); i != list_generator.end(); i++) {
		list<Particle*> aux = (*i)->generateParticles();
		list_particles.merge(aux);
	}
	for (auto i = list_particles.begin(); i != list_particles.end(); i++) {
		if (!(*i)->alive(t)) {
			delete* i;
			list_particles.erase(i);
		}
		else {
			(*i)->integrate(t);
		}
	}
	
}
