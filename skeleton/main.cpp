#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "ParticleSystem.h"
#include "AnchoredSpringFG.h"
#include "ForceGenerator.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

ParticleSystem* particleSystem;
Particle* particula;
// Initialize physics engine START DE LA ESCENA Y DE LAS FISICAS
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);//materail del suelo

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	
	particleSystem = new ParticleSystem();
}



// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds &&UPDATE DE LA ESCENA
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	/*for (auto shot : cargador) {
		if(shot!=nullptr)
		shot->integrate(t);
	}*/
	particleSystem->update(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	delete particleSystem;
	
	/*delete particula; particula = nullptr;
	for (auto shot : cargador) {
		if (shot != nullptr)
			delete shot;
	}*/
}

// Function called when a key is pressed INTERACIONAR CON TECLADO
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
	case 'M':
		particleSystem->generateHosepipeSystem();
		break;
	case 'N':
		particleSystem->generateFogSystem();
		break;
	case 'O':
		particleSystem->generateWindSystem();
		break;
	case 'P':
		particleSystem->generateWhirlSystem();
		break;
	case 'U':
		particleSystem->generateExplosionSystem();
		break;
	case 'Y':
		particleSystem->explosion(true);
		break;
	case 'I':
		particleSystem->explosion(false);
		break;
	case 'C':
		particleSystem->explosion(false);
		break;
	case 'L':
		particleSystem->generateFlamesSystem();
		break;
	case 'E':
		particleSystem->shootFirework(Firework::BASIC);
		break;
	case 'R':
		particleSystem->shootFirework(Firework::LINEAR);
		break;
	case 'T':
		particleSystem->shootFirework(Firework::CIRCULAR);
		break;
	case 'J':
		particleSystem->generateSpringSystem();
		break;
	case 'F':
		particleSystem->generateRocketSystem();
		break;
	case '+':
		particleSystem->increaseDesTip(Vector3(1.0, 1.0, 0.0));
		break;
	case '-':
		particleSystem->increaseDesTip(Vector3(-1.0, -1.0, 0.0));
		break;
	case '1':
		dynamic_cast<AnchoredSpringFG*>(particleSystem->getForceGenerator("Anchored").get())->increaseK(0.5);
		break;
	case '2':
		particleSystem->getForceGenerator("WindAnchored")->setActive(true, 5.0);			
		break;
	case '3':
		particleSystem->generateElasticBandSystem();
		break;
	}
	//case 'H': //Bola de fuego
	//{
	//	auto bullet = new Particle(camera.p, GetCamera()->getDir() * 30, Vector4(255 / 250.0, 128 / 250.0, 0.0, 1.0), Particle::TYPE::PROYECTIL);
	//	bullet->setAcceleration(Vector3(0.0, 0.6, 0.0));
	//	bullet->setDamping(0.9);
	//	bullet->setMass(1.0);
	//	cargador.push_back(bullet);
	//	break;
	//}
	//case 'F': //Laser
	//{
	//	auto bullet = new Particle(camera.p, GetCamera()->getDir() * 30, Vector4(135 / 250.0, 206 / 250.0, 250 / 250.0, 1.0),Particle::TYPE::PROYECTIL);
	//	bullet->setAcceleration(Vector3(0.0, 0.0, 0.0));
	//	bullet->setDamping(0.99);
	//	bullet->setMass(0.1);
	//	cargador.push_back(bullet);
	//	break;
	//}
	
	//}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}