#include "WorldManager.h"
#include "PxRigidStatic.h"
#include "RigidGenerator.h"
#include "GaussianRigidGenerator.h"
#include "ExplosionForceGenerator.h"
#include "TorqueForce.h"
#include "WindGenerator.h"
#include "AnchoredSpringFG.h"

WorldManager::WorldManager(PxScene* gScene, PxPhysics* gPhysics):gScene(gScene),gPhysics(gPhysics) {
	list_static = list<RigidSolid*>();
	list_dynamic = list<RigidSolid*>();
	list_generators = list<shared_ptr<RigidGenerator>>();
	list_forces = list<shared_ptr<ForceGenerator>>();
	list_materials = vector<PxMaterial*>(); list_materials.push_back(nullptr);
	rfr = new RigidForceRegistry();
	currentNum = 0;
	CreateGolfBall({ 0,10,0 }, 1.5, { 0,0,0 }, { 1.0,1.0,1.0,1.0 });
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
	list_materials.clear();
	eliminarCuerpo(golfBall);
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

void WorldManager::setNewMaterial(PxMaterial* x){
	list_materials.push_back(x);
}

void WorldManager::eliminarCuerpo(RigidSolid* cuerpo){
	//DeregisterRenderItem(cuerpo->item);
	rfr->deleteRigidRegistry(static_cast<PxRigidDynamic*>(cuerpo->solidType));
	cuerpo->solidType->release();
	DeregisterRenderItem(cuerpo->item);
	delete cuerpo;
	currentNum--;
}

void WorldManager::vaciarEntidades(){
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
	delete rfr;
	rfr = new RigidForceRegistry();
}

void WorldManager::vaciarGeneradores(){
	list_generators.clear();
}

PxRigidStatic* WorldManager::addStaticBox(Vector3 pos, Vector3 tam, Vector4 color,PxQuat q, int idMaterial){
	if (!CanGenerateObject())return nullptr;
	currentNum++;
	PxRigidStatic* obj = gPhysics->createRigidStatic(PxTransform(pos,q));
	PxShape* shape = CreateShape(PxBoxGeometry(tam),list_materials[idMaterial]);
	obj->attachShape(*shape);
	 auto item=new RenderItem(shape, obj, color);
	gScene->addActor(*obj);
	RigidSolid* rg= new RigidSolid();
	rg->solidType = obj; rg->timeAlive = -1; rg->item= item;
	list_static.push_back(rg);
	return obj;
}

PxRigidDynamic* WorldManager::addDynamicBall(Vector3 pos, double tam, Vector3 vel, Vector4 color){
	if (!CanGenerateObject())return nullptr; 
	currentNum++;

	PxRigidDynamic* new_solid;
	new_solid = gPhysics->createRigidDynamic(PxTransform(pos));
	new_solid->setLinearVelocity(vel);
	new_solid->setAngularVelocity({ 0,0,0 });

	auto shape = CreateShape(PxSphereGeometry(tam)); new_solid->attachShape(*shape);
	//PxRigidBodyExt::setMassAndUpdateInertia(*new_solid, 100.0);

	auto x=new RenderItem(shape, new_solid, color);
	gScene->addActor(*new_solid);
	
	//new_solid->setLinearDamping(0.999);
	RigidSolid* rg = new RigidSolid();
	rg->solidType = new_solid; rg->timeAlive = -1; rg->item = x;

	list_dynamic.push_back(rg);	
	return new_solid;
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

PxMaterial* WorldManager::getMaterial(int id){
	if(id>list_materials.size()&&list_materials[id]==nullptr)domain_error("Material INEXISTENTE");

	return list_materials[id];
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

void WorldManager::systemWind(string name,Vector3 pos,Vector3 dir,Vector3 tam,float range){
	auto it = getRigidGenerator(name);
	if (it != nullptr)it->changeActive();
	else {
		//{ -50,1,50 } { 50,0,50 } posModle: { 0,50,0 } range 200
		auto x = shared_ptr<RigidGenerator>(new GaussianRigidGenerator(this, DYNAMIC,tam , dir/8,
			1.0, 10.0, false)); x->setName(name); list_generators.push_back(x);
		x->setRegistry(rfr);
		PxRigidDynamic* new_solid;
		new_solid = gPhysics->createRigidDynamic(PxTransform(pos)); new_solid->setName("molde");
		new_solid->setLinearVelocity({ 0,0,0 });
		new_solid->setAngularVelocity({ 0,0,0 });
		auto shape = CreateShape(PxSphereGeometry(2.0));
		new_solid->attachShape(*shape);
		PxRigidBodyExt::setMassAndUpdateInertia(*new_solid, 50.0);
		//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
		auto xy = new RenderItem(shape, new_solid, { 0,0,1,1 });
		RigidSolid* rg = new RigidSolid();
		rg->solidType = new_solid; rg->timeAlive = 5.5; rg->maxTimeAlive = 5.5; rg->item = xy;
		x->setBody(rg);
		x->setPhysx(gPhysics);

		auto it2 = shared_ptr<ForceGenerator>(new WindGenerator(dir, 5, 5, pos, range)); it2->setName(name+"-force");
		rfr->addRegistry(it2, static_cast<PxRigidDynamic*>(rg->solidType));
		rg->forcesNames.push_back(it2->getName());
		list_forces.push_back(it2);
		x->setNumGenerator(1);
	}
}

void WorldManager::CreateGolfBall(Vector3 pos, double tam, Vector3 vel, Vector4 color){
	currentNum++;
	PxRigidDynamic* new_solid;
	new_solid = gPhysics->createRigidDynamic(PxTransform(pos));
	new_solid->setLinearVelocity(vel);
	new_solid->setAngularVelocity({ 0,0,0 });
	new_solid->setName("GOLFBALL");

	auto shape = CreateShape(PxSphereGeometry(tam)); new_solid->attachShape(*shape);
	PxRigidBodyExt::setMassAndUpdateInertia(*new_solid, tam);

	auto x = new RenderItem(shape, new_solid, color);
	gScene->addActor(*new_solid);

	new_solid->setLinearDamping(0.5);
	new_solid->setAngularDamping(0.5);
	golfBall = new RigidSolid();
	golfBall->solidType = new_solid; golfBall->timeAlive = -1; golfBall->item = x;
}

void WorldManager::createCorridorVertical(Vector3 pos, Vector3 tam,int idMaterial, Vector4 colorSuelo ){
	Vector3 suelo = tam; suelo.y = 1; suelo.z = tam.z / 2;
	addStaticBox(pos, suelo, colorSuelo,{0,0,0,1}, idMaterial);//suelo 

	suelo = tam; suelo.z = tam.z / 5; suelo.z /= 3;
	Vector3 pared = pos; pared.z = pos.z + tam.z / 2;
	addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared

	pared = pos; pared.z=pos.z- tam.z/2;
	addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared
}

void WorldManager::createCorridorHorizontal(Vector3 pos, Vector3 tam, int idMaterial, Vector4 colorSuelo) {
	Vector3 suelo = tam; suelo.y = 1; suelo.x = tam.x / 2;
	addStaticBox(pos, suelo,colorSuelo, { 0,0,0,1 }, idMaterial);//suelo 

	suelo = tam; suelo.x = tam.x / 5; suelo.x /= 3;
	Vector3 pared = pos; pared.x = pos.x + tam.x / 2;
	addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared

	pared = pos; pared.x = pos.x - tam.x / 2;
	addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared

}

void WorldManager::inicioCorridor(Vector3 pos, Vector3 tam, bool front, int idMaterial, Vector4 colorSuelo) {
	createCorridorVertical(pos, tam,idMaterial,colorSuelo);
	Vector3 pared = pos; 
	Vector3 suelo = tam; suelo.x = tam.x / 5; suelo.x /= 4; suelo.x = suelo.x/1.5;
	suelo.z /= 1.765;
	if (front) {
		pared.x = pos.x + tam.x;
		addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared
	}
	else {
		pared.x = pos.x - tam.x;
		addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared
	}
}

void WorldManager::curvaDerechaCorridor(Vector3 pos, Vector3 tam,bool front, int idMaterial, Vector4 colorSuelo) {
	Vector3 suelo = tam; suelo.y = 1; suelo.z = tam.z / 2;suelo.x-=tam.x/4;
	addStaticBox(pos, suelo, colorSuelo, {0,0,0,1},idMaterial);//suelo 
	suelo = tam; suelo.z = tam.z / 5; suelo.z /= 3; suelo.x -= tam.x / 4;

	Vector3 pared = pos; pared.z = pos.z + tam.z / 2;
	addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared

	pared = pos;
	suelo = tam; 
	suelo.x = tam.x / 5; suelo.x /= 3;
	suelo.z /= 1.765;
	
	if (front) {
		pared.x = pos.x + tam.x- tam.x / 4;
		addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared
	}
	else {
		pared.x = pos.x - tam.x+ tam.x / 4;
		addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared
	}
}

void WorldManager::curvaIzquierdaCorridor(Vector3 pos, Vector3 tam,bool front,int idMaterial, Vector4 colorSuelo) {
	Vector3 suelo = tam; suelo.y = 1; suelo.z = tam.z / 2; suelo.x -= tam.x / 4;
	addStaticBox(pos, suelo, colorSuelo,{0,0,0,1},idMaterial);//suelo 
	suelo = tam; suelo.z = tam.z / 5; suelo.z /= 3; suelo.x -= tam.x / 4;

	Vector3 pared = pos; pared.z = pos.z - tam.z / 2;
	addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared

	pared = pos;
	suelo = tam;
	suelo.x = tam.x / 5; suelo.x /= 3;
	suelo.z /= 1.765;
	if (front) {
		pared.x = pos.x - tam.x/4+tam.x;
		addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared
	}
	else {
		pared.x = pos.x - tam.x+ tam.x / 4;
		addStaticBox(pared, suelo, { 0.50,0.25,0.0,1.0 });//Pared
	}
}

void WorldManager::rampa(Vector3 pos, Vector3 tam, PxQuat q, int idMaterial, Vector4 colorSuelo) {
	Vector3 suelo = tam; suelo.y = 1;
	addStaticBox(pos, suelo, colorSuelo, q,idMaterial);
}

void WorldManager::sueloLiso(Vector3 pos, Vector3 tam,int idMaterial, Vector4 colorSuelo) {
	Vector3 suelo = tam; suelo.y = 1;
	addStaticBox(pos, suelo, colorSuelo,{0,0,0,1},idMaterial);//suelo 
	//feometria Geometry_
}


void WorldManager::sueloBaseCollision(Vector3 pos, Vector3 tam, char* Name){
	Vector3 suelo = tam; suelo.y = 1;
	addStaticBox(pos, suelo, { 0.6,1.0,0.6,1.0 });//suelo 
	
	list_static.back()->solidType->setName(Name);
}

void WorldManager::hoyo(char* name,Vector3 pos, Vector3 tam){
	Vector3 pos2 = pos, aux = tam; pos2.y += 0.90;
	auto hoyoo=addStaticBox(pos2, tam, { 1.0,0.3,0.3,1.0 });
	pos2 = pos;
	pos2.y += 15;
	addStaticBox(pos2, { 0.50,7.5,0.50 }, { 0.5,0.5,0.5,1.0 });
	pos2.y += 3.0; pos2.x += 3.0; pos2.z -= 3;
	addStaticBox(pos2, { 4.0,2.0,0.25 }, { 0.8,0.5,0.9,1.0 },PxQuat(180.0, {0,1,0}));
	hoyoo->setName(name);
}

void WorldManager::crearObjectoVacio(Vector3 pos, Vector3 tam, string size){

}

void WorldManager::muelleDinamico(string name, Vector3 pos,double k,double resting,bool z){
	auto f3 = shared_ptr<ForceGenerator>(new AnchoredSpringFG(k, resting, pos));
	f3->setName(name);	list_forces.push_back(f3);
	Vector3 auxPos = pos; 
	if(!z)auxPos.z += resting;
	else auxPos.x += resting;
	auto x = addDynamicBall(auxPos, 2.0, Vector3(0), {0.8,0.26,0.5,1.0});

	PxRigidBodyExt::setMassAndUpdateInertia(*x, 10.0);
	//list_dynamic.front()->forcesNames.push_back(name);
	rfr->addRegistry(f3, x);
}