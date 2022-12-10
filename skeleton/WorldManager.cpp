#include "WorldManager.h"
#include "PxRigidStatic.h"
#include "RigidGenerator.h"
#include "GaussianRigidGenerator.h"
#include "ExplosionForceGenerator.h"
#include "TorqueForce.h"
#include "WindGenerator.h"

WorldManager::WorldManager(PxScene* gScene, PxPhysics* gPhysics):gScene(gScene),gPhysics(gPhysics) {
	list_static = list<RigidSolid*>();
	list_dynamic = list<RigidSolid*>();
	rfr = new RigidForceRegistry();
	currentNum = 0;
	addStaticBox({ 0,0,0 }, { 200,1,200 }, { 0.0,1.0,0.0,1.0 });
	addStaticBox({ 200,0,0 }, { 1,100,200 }, { 0.0,1.0,0.0,1.0 });
	addStaticBox({ -200,0,0 }, { 1,100,200 }, { 0.0,1.0,0.0,1.0 });
	addStaticBox({ 0,0,200 }, { 200,100,1 }, { 0.0,1.0,0.0,1.0 });
	addStaticBox({ 0,0,-200 }, { 200,100,1 }, { 0.0,1.0,0.0,1.0 });
	addDynamicBall({ 0,100,0 }, 5.0, { 50,-20,0 }, { 1.0,1.0,1.0,1.0 });
	
}

WorldManager::~WorldManager(){
	while (!list_dynamic.empty()) {
		eliminarCuerpo(list_dynamic.front());
		//delete list_dynamic.front()->item;
		list_dynamic.pop_front();
	}
	while (!list_static.empty()) {
		eliminarCuerpo(list_static.front());
		//delete list_static.front()->item;
		list_static.pop_front();
	}
	list_forces.clear();
	list_generators.clear();
	delete rfr;
}

void WorldManager::update(double duration){

	for (auto t = list_generators.begin(); t != list_generators.end(); t++){
		auto gen = *t;
		if (gen->isActive()) {
			if (gen->type == STATIC)gen->generateRigid(list_static);
			else gen->generateRigid(list_dynamic);
		}
	}

	auto p = list_dynamic.begin();
	while ( p != list_dynamic.end()) {
		auto cuerpo = *p;
		if (cuerpo->timeAlive == -1) p++;
		else{
			cuerpo->timeAlive -= duration;
			if (cuerpo->timeAlive <= 0) {
				eliminarCuerpo(cuerpo);
				p = list_dynamic.erase(p);
			}
			else p++;
		}
	}
	auto pa = list_static.begin();
	while ( pa != list_static.end()) {
		auto cuerpo = *pa;
		if (cuerpo->timeAlive == -1)pa++;	
		else {
			cuerpo->timeAlive -= duration;
			if (cuerpo->timeAlive <= 0) {
				eliminarCuerpo(cuerpo);
				pa = list_static.erase(pa);
			}
			else pa++;
		}
	}
	rfr->updateForces(duration);
}

void WorldManager::eliminarCuerpo(RigidSolid* cuerpo){
	DeregisterRenderItem(cuerpo->item);
	delete cuerpo;
	currentNum--;
}

void WorldManager::setMaterialToObject(PxRigidActor* x, Vector3 mat){
	dynamic_cast<PxRigidDynamic*>(x)->setMassSpaceInertiaTensor(mat);
}

void WorldManager::addStaticBox(Vector3 pos, Vector3 tam, Vector4 color){
	if (!CanGenerateObject())return;
	currentNum++;

	PxRigidStatic* obj = gPhysics->createRigidStatic(PxTransform(pos));
	PxShape* shape = CreateShape(PxBoxGeometry(tam));
	obj->attachShape(*shape);
	 auto item=new RenderItem(shape, obj, color);
	gScene->addActor(*obj);
	RigidSolid* rg= new RigidSolid();
	rg->solidType = obj; rg->timeAlive = -1; rg->item= item;
	list_static.push_back(rg);
}

void WorldManager::addDynamicBall(Vector3 pos, double tam, Vector3 vel, Vector4 color){
	if (!CanGenerateObject())return; 
	currentNum++;

	PxRigidDynamic* new_solid;
	new_solid = gPhysics->createRigidDynamic(PxTransform(pos));
	new_solid->setLinearVelocity(vel);
	new_solid->setAngularVelocity({ 0,0,0 });

	auto shape = CreateShape(PxSphereGeometry(tam)); new_solid->attachShape(*shape);
	PxRigidBodyExt::setMassAndUpdateInertia(*new_solid, 100.0);

	auto x=new RenderItem(shape, new_solid, color);
	gScene->addActor(*new_solid);
	
	//new_solid->setLinearDamping(0.999);
	RigidSolid* rg = new RigidSolid();
	rg->solidType = new_solid; rg->timeAlive = -1; rg->item = x;
	auto it = shared_ptr<ForceGenerator>(new ExplosionForceGenerator(200, 1000, { 0,50,0 }, 5.0));

	list_dynamic.push_back(rg);	
}

shared_ptr<RigidGenerator> WorldManager::getRigidGenerator(string name) {
	for (auto i = list_generators.begin(); i != list_generators.end(); i++) {
		if ((*i)->getName() == name)return (*i);
	}
	//si no lo encuentra
	return nullptr;
}

shared_ptr<ForceGenerator> WorldManager::getForceGenerator(string name) {
	for (auto i = list_forces.begin(); i != list_forces.end(); i++) {
		if ((*i)->getName() == name)return (*i);
	}
	//si no lo encuentra
	return nullptr;
}

void WorldManager::systemOne(){
	auto it = getRigidGenerator("Torque1");
	if (it != nullptr)it->changeActive();
	else {
		auto x = shared_ptr<RigidGenerator>(new GaussianRigidGenerator(this, DYNAMIC, { -50,1,50 }, { 20,10,20 },
			1.0, 10.0, true)); x->setName("Torque1"); list_generators.push_back(x);
		x->setRegistry(rfr);
		PxRigidDynamic* new_solid;
		new_solid = gPhysics->createRigidDynamic(PxTransform({ 0,100,0 })); new_solid->setName("molde");
		new_solid->setLinearVelocity({ 0,0,0 });
		new_solid->setAngularVelocity({ 0,0,0 });
		//new_solid->setLinearDamping(0.999);
		auto shape = CreateShape(PxBoxGeometry(Vector3(3.5)));
		new_solid->attachShape(*shape);
		//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
		PxRigidBodyExt::setMassAndUpdateInertia(*new_solid, 100.0);
		auto xy = new RenderItem(shape, new_solid, { 0,0,1,1 });
		RigidSolid* rg = new RigidSolid();
		rg->solidType = new_solid; rg->timeAlive = 25; rg->maxTimeAlive = 25; rg->item = xy;
		x->setBody(rg);
		x->setPhysx(gPhysics);

		auto it2 = shared_ptr<ForceGenerator>(new TorqueForce({ 0,0,0 }, 10, 100)); it2->setName("TORQUE");
		rfr->addRegistry(it2, static_cast<PxRigidDynamic*>(rg->solidType));
		rg->forcesNames.push_back(it2->getName());
		list_forces.push_back(it2);
	}
}

void WorldManager::systemWind(){
	auto it = getRigidGenerator("Wind1");
	if (it != nullptr)it->changeActive();
	else {
		auto x = shared_ptr<RigidGenerator>(new GaussianRigidGenerator(this, DYNAMIC, { -50,1,50 }, { 50,0,50 },
			1.0, 10.0, true)); x->setName("Wind1"); list_generators.push_back(x);
		x->setRegistry(rfr);
		PxRigidDynamic* new_solid;
		new_solid = gPhysics->createRigidDynamic(PxTransform({ 0,50,0 })); new_solid->setName("molde");
		new_solid->setLinearVelocity({ 0,0,0 });
		new_solid->setAngularVelocity({ 0,0,0 });
		auto shape = CreateShape(PxSphereGeometry(2.0));
		new_solid->attachShape(*shape);
		//PxRigidBodyExt::setMassAndUpdateInertia(*new_solid, 50.0);
		//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
		auto xy = new RenderItem(shape, new_solid, { 0,0,1,1 });
		RigidSolid* rg = new RigidSolid();
		rg->solidType = new_solid; rg->timeAlive = 15; rg->maxTimeAlive = 5; rg->item = xy;
		x->setBody(rg);
		x->setPhysx(gPhysics);

		auto it2 = shared_ptr<ForceGenerator>(new WindGenerator({ 0,0,10 }, 5, 5, { 0,0,0 }, 200)); it2->setName("WIND1");
		rfr->addRegistry(it2, static_cast<PxRigidDynamic*>(rg->solidType));
		rg->forcesNames.push_back(it2->getName());
		list_forces.push_back(it2);
	}
}
