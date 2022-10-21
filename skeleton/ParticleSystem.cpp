#include "ParticleSystem.h"
#include "GaussianParticleGen.h"
#include "UniformParticleGenenerator.h"
#include <iostream>
ParticleSystem::ParticleSystem() {
	list_generator.push_back(new UniformParticleGenerator(Vector3(16,16.0,16.0), Vector3(0.0, 20.0, 0.0)));
	list_particles = list<Particle*>();
	/*list_particles.push_back(new Particle(Vector3(15.0, 15.0, 0.0), Vector3(20.0, 20.0, 0.0), 
		Vector3(0.1, -2.8, 0.0), 0.0, 10.0, Particle::UNUSED));*/
	
}
ParticleSystem::~ParticleSystem() {
	while (!list_generator.empty()) {
		delete list_generator.front();
		list_generator.pop_front();
	}
	while( !list_particles.empty()) {	
		delete list_particles.front();
		list_particles.pop_front();			
	}
	
}

void ParticleSystem::update(double t) {
	for (auto i = list_generator.begin(); i != list_generator.end(); i++) {
		 (*i)->generateParticles(list_particles);	
		 std::cout << list_particles.size() << std::endl;
	}
	for (auto i = list_particles.begin(); i != list_particles.end(); i++) {
		if (!(*i)->alive(t)) {
			auto aux = i;
			delete* aux;
			i++;
			list_particles.erase(aux);			
		}
		else {
			(*i)->integrate(t);
		}
	}
	
}
ParticleGenerator* ParticleSystem::getParticleGenerator(string name) {
	for (auto i = list_generator.begin(); i != list_generator.end(); i++) {
		if ((*i)->getName() == name)return (*i);
	}
	//si no lo encuentra
	return nullptr;
}
