#include "ParticleSystem.h"
#include "GaussianParticleGen.h"
#include "UniformParticleGenenerator.h"
#include <iostream>
ParticleSystem::ParticleSystem() {
	//list_generator.push_back(shared_ptr<ParticleGenerator>(new UniformParticleGenerator(Vector3(16,16.0,16.0), Vector3(0.0, 20.0, 0.0))));
	list_generator.push_back(shared_ptr<ParticleGenerator>(new UniformParticleGenerator(Vector3(2.0,2.0,2.0), Vector3(10.0, -5.0, 10.0))));
	list_generator.front()->setActive(false);
	list_particles = list<Particle*>();
	/*list_particles.push_back(new Particle(Vector3(15.0, 15.0, 0.0), Vector3(20.0, 20.0, 0.0), 
		Vector3(0.1, -2.8, 0.0), 0.0, 10.0, Particle::UNUSED));*/
	
}
ParticleSystem::~ParticleSystem() {
	while (!list_generator.empty()) {
		//delete list_generator.front();
		list_generator.pop_front();
	}
	while( !list_particles.empty()) {	
		delete list_particles.front();
		list_particles.pop_front();			
	}
	
}

void ParticleSystem::update(double t) {
	for (auto g = list_generator.begin(); g != list_generator.end(); ++g)
	{
		if ((*g)->isActive())
			(*g)->generateParticles(list_particles);
	}

	for (auto p = list_particles.begin(); p != list_particles.end(); ++p)
	{
		if ((*p)->alive(t))
				(*p)->update(t);
	}
	auto p = list_particles.begin();
	while (p != list_particles.end()) {
		if (!(*p)->alive(t)) {
			onParticleDeath((*p));
			delete *p;
			p = list_particles.erase(p);
		}
		else p++;
	}
	
}
shared_ptr<ParticleGenerator> ParticleSystem::getParticleGenerator(string name) {
	for (auto i = list_generator.begin(); i != list_generator.end(); i++) {
		if ((*i)->getName() == name)return (*i);
	}
	//si no lo encuentra
	return nullptr;
}

void ParticleSystem::generateFireworkSystem(){

}

void ParticleSystem::shootFirework(Firework::FIREWORK_TYPE type){
	switch (type){
	case Firework::BASIC:
		cout << "dfsdfsdf\n";
		auto x = new Firework(Vector3(0, 0, 0), Vector3(0, 15, 0), Vector3(0, 1, 0), 0.9999, 1.0, Firework::BASIC);
		x->addGenerator(list_generator.front());
		x->setTimeAlive(8.0);
		list_particles.push_back(x);
		auto xy = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.9999, 1.0, Particle::UNUSED); xy->setTimeAlive(100000);
		list_particles.push_back(xy);
	break;
	}
}

void ParticleSystem::onParticleDeath(Particle* pt) {
	Firework* fk = dynamic_cast<Firework*>(pt);
	if (fk != nullptr) {
		fk->explode(list_particles);
	}
}
