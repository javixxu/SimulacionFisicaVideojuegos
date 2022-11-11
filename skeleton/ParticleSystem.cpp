#include "ParticleSystem.h"
#include "GaussianParticleGen.h"
#include "UniformParticleGenenerator.h"
#include "CircleGenerator.h"
#include <iostream>
#include "RocketGenerator.h"
#include "GravityForceGenerator.h"
#include "ParticleDragGenerator.h"

ParticleSystem::ParticleSystem() {
	list_particles = list<Particle*>();
	pForceRegistry =new ParticleForceRegistry();
	list_forces = list<shared_ptr<ForceGenerator>>();

	auto xy = new Particle(Vector3(0), Vector3(0)); xy->setTimeAlive(1000000000); list_particles.push_back(xy); xy->setMass(2.0);
	shared_ptr<ParticleGenerator>it= shared_ptr<ParticleGenerator>(new GaussianParticleGen(Vector3(.0, .0, .0), Vector3(2.50, -2.0, 2.50),1.0));
	list_generator.push_back(it);
	(*it).changeActive();

	auto DragGenerator = shared_ptr<ForceGenerator>(new ParticleDragGenerator(0.5,0));
	list_forces.push_back(DragGenerator); DragGenerator.get()->setName("DragGenerator");
	

	auto gravityG = shared_ptr<ForceGenerator>(new GravityForceGenerator(gravity));
	list_forces.push_back(gravityG);gravityG.get()->setName("Gravity");
	pForceRegistry->addRegistry(gravityG, xy);

	gravityG = shared_ptr<ForceGenerator>(new GravityForceGenerator(Vector3(0, 6, 0)));
	list_forces.push_back(gravityG); gravityG.get()->setName("Gravity2");

	

	it = shared_ptr<ParticleGenerator>(new CircleGenerator(30,10));
	list_generator.push_back(it); (*it).setName("CircleRockets"); it->changeActive();

	generateFireworkSystem();
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
	for (int i = 0; i < fireworks_pool.size(); i++){
		delete fireworks_pool[i];
	}
	fireworks_pool.clear();
	
}

void ParticleSystem::update(double t) {
	pForceRegistry->updateForces(t);
	for (auto g = list_generator.begin(); g != list_generator.end(); ++g){
		if ((*g)->isActive())
			(*g)->generateParticles(list_particles);
	}

	for (auto p = list_particles.begin(); p != list_particles.end(); ++p){		
		if ((*p)->update(t)) {
			if((*p)->isFire())
				(*p)->changeSize((*p)->getTimeAlive() * t, (*p)->getTransform(), (*p)->getColor());
		}
	}
	auto p = list_particles.begin();
	while (p != list_particles.end()) {
		if (!(*p)->alive()) {
			pForceRegistry->deleteParticleRegistry((*p));
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

shared_ptr<ForceGenerator> ParticleSystem::getForceGenerator(string name) {
	for (auto i = list_forces.begin(); i != list_forces.end(); i++) {
		if ((*i)->getName() == name)return (*i);
	}
	//si no lo encuentra
	return nullptr;
}

void ParticleSystem::generateFireworkSystem(){
	//se rellena el firework_pool(debemos generalos a tomar por culo) y se inicializa el fireworksystrem
	auto circle = list_generator.begin(); circle++; 
	//BASIC
	auto x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 25, 0), Vector3(0, 1, 0), 0.9999, 1.0, Firework::BASIC);
	x->addGenerator(list_generator.front()); 
	x->setTimeAlive(4.0);
	fireworks_pool.push_back(x); x->setNumGen(0);

	//LINEAR
	x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 30, 0), Vector3(0, 2, 0), 0.9999, 1.0, Firework::LINEAR);
	x->addGenerator(list_generator.front()); 
	x->setTimeAlive(4.0);
	fireworks_pool.push_back(x); x->setNumGen(0);

	//CIRCULAR
	x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 40, 0), Vector3(0, 2, 0), 0.9999, 1.0, Firework::CIRCULAR);
	x->addGenerator(*circle); x->setNumGen(0);
	x->setTimeAlive(4.0);
	fireworks_pool.push_back(x);
}

void ParticleSystem::shootFirework(Firework::FIREWORK_TYPE type){
	Firework* x;
	switch (type){
	case Firework::BASIC:
		cout << "BASICO\n";
		x = fireworks_pool.at(0)->clone();
		x->setPosition({ 0,0,0 });
	
		list_particles.push_back(x);
		
	break;
	case Firework::LINEAR:
		cout << "LINEAR\n";
		x = fireworks_pool.at(1)->clone();
		x->setPosition({ 0,0,0 });

		list_particles.push_back(x);

		break;
	case Firework::CIRCULAR:
		cout << "CIRCULAR\n";
		x = fireworks_pool.at(2)->clone();
		x->setPosition({ 0,0,0 });

		list_particles.push_back(x);
		break;
	}
}

void ParticleSystem::onParticleDeath(Particle* pt) {
	Firework* fk = dynamic_cast<Firework*>(pt);
	if (fk != nullptr) {
		fk->explode(list_particles);
	}
}

void ParticleSystem::generateHosepipeSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("HosePipeSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GaussianParticleGen(Vector3(0.2, 0.2, 0.0),Vector3(4.0, 1.0, 4.0), 0.8);
		s->setName("HosePipeSystem");
		Particle* p = new Particle(Vector3(-50.0, 0.0, 0.0),Vector3(30.0, 15.0, -30.0), {0,0,0}, 0.999, 2.0,
			Particle::UNUSED, Vector4(0.0, 0.7, 0.96, 1.0), 1.0); 
		p->setMass(.3);
		p->setTimeAlive(2.0);
		pForceRegistry->addRegistry(getForceGenerator("Gravity"), p);
		s->setParticle(p);
		s->addParticleForceRegistry(pForceRegistry);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateFogSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("FogSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GaussianParticleGen(Vector3(20.0, 8.0, 5.0), Vector3(0.2, 0.1, 0.2), 0.6);
		s->setName("FogSystem");		
		Particle* p = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 3.0, -2.5), Vector3(0, 0, 0), 0.75,0.3);
		p->setColor(Vector4(1.0, 1.0, 1.0, 0.25));
		pForceRegistry->addRegistry(getForceGenerator("DragGenerator"), p);
		p->setTimeAlive(1.0);		
		s->setParticle(p);
		s->setNumGenerator(30);
		s->addParticleForceRegistry(pForceRegistry);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateFlamesSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("FlamesSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GaussianParticleGen( Vector3(10.0, 5.5, 5.0),Vector3(0.2, 0.1, 0.2), 0.3);
		s->setName("FlamesSystem");
		
		Particle* p = new Particle(Vector3(1.0, 5.0, -1.0), Vector3(0.0, 10.0, 0.0), Vector3(0, 0, 0), 0.9,0.5);
		p->setColor(Vector4(255 / 250.0, 128 / 250.0, 0.0, 0.5));
		p->setTimeAlive(3.0);
		pForceRegistry->addRegistry(getForceGenerator("Gravity2"), p);
		p->setIsFire(true);
		s->setParticle(p);
		s->setNumGenerator(30);
		s->addParticleForceRegistry(pForceRegistry);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateCircleSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("CircleSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new CircleGenerator(50,15); s->setName("CircleSystem");
		Particle* p = new Particle(Vector3(0.0, 20.0, 0.0),Vector3(0.0, 0.0, 0.0), gravity, 0.999);
		p->setColor(Vector4(255 / 250.0, 128 / 250.0, 0.0, 1.0));
		p->setTimeAlive(8.0);

		s->setParticle(p);
		s->setNumGenerator(30);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateRocketSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("RocketSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new RocketGenerator({ 0,0,0 }, { 10,-10,0 }); s->setName("RocketSystem");
		Particle* p = new Particle(Vector3(0.0, 0.0, 0.0), Vector3(5.0, 20.0, 0.0), gravity, 0.999);
		s->setTypesRockets(fireworks_pool);
		p->setColor(Vector4(255 / 250.0, 128 / 250.0, 0.0, 1.0));
		p->setTimeAlive(8.0);

		s->setParticle(p);
		s->setNumGenerator(1);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::increaseDesTip(Vector3 increase)
{
	for (auto it = list_generator.begin(); it != list_generator.end(); it++) {
		if ((*it)->isActive()) {
			GaussianParticleGen* f = dynamic_cast <GaussianParticleGen*>((*it).get());
			if (f != nullptr) f->increaseDesTipVel(increase);
		}
	}
}