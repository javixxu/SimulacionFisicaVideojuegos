#include "ParticleSystem.h"
#include "GaussianParticleGen.h"
#include "UniformParticleGenenerator.h"
#include "CircleGenerator.h"
#include <iostream>
#include "RocketGenerator.h"
#include "GravityForceGenerator.h"
#include "WindGenerator.h"
#include "WhirlWindGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "ElasticBand.h"
#include "FloatBounceForce.h"

ParticleSystem::ParticleSystem() {
	list_particles = list<Particle*>();
	pForceRegistry =new ParticleForceRegistry();
	list_forces = list<shared_ptr<ForceGenerator>>();

	shared_ptr<ParticleGenerator>it= shared_ptr<ParticleGenerator>(new GaussianParticleGen(Vector3(15.0, 10.0, .0), Vector3(2.50, -2.0, 2.50),1.0,0.0,false));
	list_generator.push_back(it);
	(*it).changeActive();

	it = shared_ptr<ParticleGenerator>(new CircleGenerator(30,10));
	list_generator.push_back(it); (*it).setName("CircleRockets"); it->changeActive();

	generateFireworkSystem();

	auto gravityG = shared_ptr<ForceGenerator>(new GravityForceGenerator(gravity));
	list_forces.push_back(gravityG); gravityG.get()->setName("Gravity");

	gravityG = shared_ptr<ForceGenerator>(new GravityForceGenerator(Vector3(0, 6, 0)));
	list_forces.push_back(gravityG); gravityG.get()->setName("Gravity2");

	auto windGenerator = shared_ptr<ForceGenerator>(new WindGenerator({ 15,4,-8 }, 5.0, 0.0, {0,-75,0},100));
	list_forces.push_back(windGenerator); windGenerator.get()->setName("WindGenerator");

	auto whirlGenerator = shared_ptr<ForceGenerator>(new WhirlWindGenerator({ 0,20,0 }, { 0.0,0.0,0.0 }, 1.0));
	list_forces.push_back(whirlGenerator); whirlGenerator.get()->setName("WhirlGenerator");

	auto ExplosionGenerator = shared_ptr<ForceGenerator>(new ExplosionForceGenerator(50, 900, {0,0,0},3.0));
	list_forces.push_back(ExplosionGenerator); ExplosionGenerator.get()->setName("ExplosionForceGenerator");

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

void ParticleSystem::explosion(bool activee){
	(dynamic_cast<ExplosionForceGenerator*>(getForceGenerator("ExplosionForceGenerator").get()))->OnActive(activee);
}

void ParticleSystem::generateHosepipeSystem() { //CAMBIADO Y FUNCIONA CON GRAVEDAD  NO CON ACELERACION NEGATIVA
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
		Particle* p = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 3.0, -2.5), Vector3(0, 0, 0), 0.75,0.3,
			Particle::UNUSED,Vector4(1.0, 1.0, 1.0, 0.25));
		pForceRegistry->addRegistry(getForceGenerator("WindGenerator"), p);
		pForceRegistry->addRegistry(getForceGenerator("Gravity"), p);
		p->setTimeAlive(1.0);		
		s->setParticle(p);
		s->setNumGenerator(30);
		s->addParticleForceRegistry(pForceRegistry);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateWindSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("WindSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		Particle* p = new Particle(Vector3(-50.0, 50.0, 0.0), Vector3(10.0,7.0, 0.0), Vector3(0, 0, 0), 0.75, 0.3,
			Particle::UNUSED, Vector4(0.0, 0.0, 1.0, 0.5), 1.0);
		auto s = new GaussianParticleGen(Vector3(0.0, 5.0, 5.0), Vector3(0.2, 0.1, 0.2), 0.6);
		s->setName("WindSystem");
		pForceRegistry->addRegistry(getForceGenerator("WindGenerator"), p);
		pForceRegistry->addRegistry(getForceGenerator("Gravity"), p);
		p->setTimeAlive(15.0);
		s->setParticle(p);
		s->setNumGenerator(1);
		s->addParticleForceRegistry(pForceRegistry);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateWhirlSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("WhirlSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		Particle* p = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 8.0, -2.5), Vector3(0, 0, 0), 0.75, 0.3,
			Particle::UNUSED, Vector4(1.0, 1.0, 1.0, 0.25), 0.2);
		auto s = new GaussianParticleGen(Vector3(7.0, 8.0, 5.0), Vector3(0.2, 0.1, 0.2), 0.6);
		s->setName("WhirlSystem");		
		pForceRegistry->addRegistry(getForceGenerator("WhirlGenerator"), p);
		pForceRegistry->addRegistry(getForceGenerator("Gravity"), p);
		p->setTimeAlive(4.0);
		s->setParticle(p);
		s->setNumGenerator(5);
		s->addParticleForceRegistry(pForceRegistry);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateExplosionSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGenerator("ExplosionSystem");
	if (p != nullptr) {
		p->changeActive();
		p->generateParticles(list_particles);
		p->changeActive();
	}
	else {
		Particle* p = new Particle(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), Vector3(0, 0, 0), 0.75, 1.0,
			Particle::UNUSED, Vector4(1.0, 1.0, 1.0, 0.25), 1.0);
		auto s = new GaussianParticleGen(Vector3(5.0, 5.0, 5.0), Vector3(0.0, 0.0, 0.0), 0.6,2.0);
		s->setName("ExplosionSystem");
		pForceRegistry->addRegistry(getForceGenerator("ExplosionForceGenerator"), p);
		p->setTimeAlive(4.0);
		s->setParticle(p);
		s->setNumGenerator(30);
		s->addParticleForceRegistry(pForceRegistry);
		list_generator.push_back(shared_ptr<ParticleGenerator>(s));
		s->generateParticles(list_particles);
		s->changeActive(); 
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

void ParticleSystem::generateSpringSystem() {

	Particle* p1 = new Particle({ -10.0,10.0,0.0 }, { 0.0,0.0,0.0 }, { 0,0,0 }, 0.85, 2.0, Particle::UNUSED, { 1.0,0.0,0.0,1.0 },2.0);
	Particle* p2 = new Particle({ 10.0,10.0,0.0 }, { 0.0,0.0,0.0 }, { 0,0,0 }, 0.85, 2.0, Particle::UNUSED, { 0.0,1.0,0.0,1.0 },2.0);
	p1->setMass(2.0); p2->setMass(2.0); p1->setTimeAlive(60); p2->setTimeAlive(60);
	auto it = shared_ptr<ForceGenerator>(new SpringForceGenerator(p2, 10.0, 15.0)); it->setName("SpringSystem1");
	list_forces.push_back(it);pForceRegistry->addRegistry(it, p1); 
	it = shared_ptr<ForceGenerator>(new SpringForceGenerator(p1, 10.0, 15.0)); it->setName("SpringSystem2");
	list_forces.push_back(it); pForceRegistry->addRegistry(it, p2); 
	list_particles.push_back(p1);
	list_particles.push_back(p2);
		
	Particle* p3 = new Particle({ -10.0, 20.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85,2.0,Particle::UNUSED,{ 1.0, 1.0, 1.0, 1.0 });
	p3->setTimeAlive(60); 
	auto f3 = shared_ptr<ForceGenerator>(new AnchoredSpringFG(10, 10, { 10.0, 20.0, 0.0 })); f3->setName("Anchored");
	auto f4 = shared_ptr<ForceGenerator>(new WindGenerator({ 0.0, -10.0, 0.0 })); f4->setName("WindAnchored"); f4->setActive(false);
	pForceRegistry->addRegistry(f4, p3); pForceRegistry->addRegistry(f3, p3); pForceRegistry->addRegistry(getForceGenerator("Gravity"), p3);
	list_forces.push_back(f3);
	list_forces.push_back(f4);
	list_particles.push_back(p3);
}

void ParticleSystem::generateElasticBandSystem() {
	Particle* p1 = new Particle({ -10.0, 10.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85);
	Particle* p2 = new Particle({ 10.0, 10.0, 0 }, { 0.0, 0.0, 0.0 },  { 0.0, 0.0, 0.0 }, 0.85);
	p2->setMass(2.0);
	p1->setTimeAlive(60);
	p2->setTimeAlive(60);

	auto f1 = shared_ptr<ForceGenerator>(new ElasticBand(p2,50, 10));
	pForceRegistry->addRegistry(f1, p1);
	//pfr->addRegistry(getForceGen("GravityForce"), p1);
	auto f2 = shared_ptr<ForceGenerator>(new ElasticBand(p1,50, 10));
	pForceRegistry->addRegistry(f2, p2);
	//pfr->addRegistry(getForceGen("GravityForce"), p2);
	list_forces.push_back(f1);
	list_forces.push_back(f2);
	list_particles.push_back(p1);
	list_particles.push_back(p2);
	
}

void ParticleSystem::Slinky() {
	Particle* p1 = new Particle({ 0.0, 60.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.0, Particle::UNUSED, { 1.0, 1.0, 1.0, 1.0 });
	Particle* p2 = new Particle({ 0.0, 50.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.0, Particle::UNUSED, { 0.0, 0.0, 1.0, 1.0 });
	Particle* p3 = new Particle({ 0.0, 40.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.0, Particle::UNUSED, { 0.0, 0.0, 1.0, 1.0 });
	Particle* p4 = new Particle({ 0.0, 30.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.0, Particle::UNUSED, { 0.0, 0.0, 1.0, 1.0 });
	Particle* p5 = new Particle({ 0.0, 20.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.0, Particle::UNUSED, { 0.0, 0.0, 1.0, 1.0 });
	Particle* p6 = new Particle({ 0.0, 10.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.0, Particle::UNUSED, { 0.0, 0.0, 1.0, 1.0 });

	auto f1 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p1, 50, 10)); //LA 1 CON LA 2
	pForceRegistry->addRegistry(f1, p2);

	auto f3 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p2,40, 10)); //LA 2 CON LA 3
	pForceRegistry->addRegistry(f3, p3);
	auto f4 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p3, 35, 10)); //LA 2 CON LA 3
	pForceRegistry->addRegistry(f4, p2);

	auto f5 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p3,30, 10)); //LA 3 CON LA 4
	pForceRegistry->addRegistry(f5, p4);
	auto f6 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p4, 25, 10)); //LA 3 CON LA 4
	pForceRegistry->addRegistry(f6, p3);

	auto f7 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p4,20, 10)); //LA 4 CON LA 5
	pForceRegistry->addRegistry(f7, p5);
	auto f8 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p5, 15, 10)); //LA 4 CON LA 5
	pForceRegistry->addRegistry(f8, p4);

	auto f9 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p5,10, 10)); //LA 5 CON LA 6
	pForceRegistry->addRegistry(f9, p6);
	auto f10 = shared_ptr<ForceGenerator>(new SpringForceGenerator(p6, 5, 10)); //LA 5 CON LA 6
	pForceRegistry->addRegistry(f10, p5);
	
	pForceRegistry->addRegistry(getForceGenerator("Gravity"), p2);
	pForceRegistry->addRegistry(getForceGenerator("Gravity"), p3);
	pForceRegistry->addRegistry(getForceGenerator("Gravity"), p4);
	pForceRegistry->addRegistry(getForceGenerator("Gravity"), p5);
	pForceRegistry->addRegistry(getForceGenerator("Gravity"), p6);


	list_forces.push_back(f1);list_forces.push_back(f3); 
	list_forces.push_back(f4);list_forces.push_back(f5); list_forces.push_back(f6);
	list_forces.push_back(f7);list_forces.push_back(f8);list_forces.push_back(f9);
	list_forces.push_back(f10);


	list_particles.push_back(p1); p1->setTimeAlive(60);
	list_particles.push_back(p2); p2->setTimeAlive(60);
	list_particles.push_back(p3); p3->setTimeAlive(60);
	list_particles.push_back(p4); p4->setTimeAlive(60);
	list_particles.push_back(p5); p5->setTimeAlive(60);
	list_particles.push_back(p6); p6->setTimeAlive(60);
}

void ParticleSystem::generaflotacion(){
	shared_ptr<ParticleGenerator> p = getParticleGenerator("Flotacion");
	if (p != nullptr)
		p->changeActive();
	else {
		Particle* p1 = new Particle({ 0.0, 0.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.0, Particle::UNUSED, { 0.0, 0.0, 0.80, 1.0 });
		p1->changeToBox({ 125,0.25,125 }); p1->setTimeAlive(100000.0); list_particles.push_back(p1);

		auto f1 = shared_ptr<ForceGenerator>(new FloatBounceForce(1.0, 1.0, 2.0)); auto gravedad = getForceGenerator("Gravity");
		list_forces.push_back(f1); f1->setName("FloatForce");
		auto gausian = shared_ptr<ParticleGenerator>(new GaussianParticleGen({ 50,0,50 }, { 0,0,0 }, 3.0, 0.5));
		gausian->setName("Flotacion"); list_generator.push_back(gausian);
		auto molde = new Particle({ 0,0,0 }, { 0.0,0.0,0.0 }, { 0,0,0 }, 0.999, 10.0, Particle::UNUSED, { 0.0,1.0,1.0,1.0 }, 1.0);
		gausian->setParticle(molde); molde->setTimeAlive(5.0);
		gausian->setNumGenerator(1);
		gausian->addParticleForceRegistry(pForceRegistry);
		pForceRegistry->addRegistry(f1, molde);
		pForceRegistry->addRegistry(gravedad, molde);
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

void ParticleSystem::increaseHeightFloatSystem(float x){
	FloatBounceForce* w = dynamic_cast<FloatBounceForce*>(getForceGenerator("FloatForce").get());
	w->setHeight(w->getHeight() + x);
}

void ParticleSystem::increaseVolumeFloatSystem(float x){
	FloatBounceForce* w = dynamic_cast<FloatBounceForce*>(getForceGenerator("FloatForce").get());
	w->setVolume(w->getVolume() + x);
}
