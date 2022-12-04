#include "WorldManager.h"
#include "PxRigidStatic.h"
#include "PxRigidStatic.h"

WorldManager::WorldManager(PxScene* gScene, PxPhysics* gPhysics):gScene(gScene),gPhysics(gPhysics) 
{
	list_static = list<RigidSolid>();
	list_dynamic = list<RigidSolid>();

}

WorldManager::~WorldManager(){
	
}

void WorldManager::update(double duration){
	for (auto p = list_static.begin(); p != list_static.end(); ++p) {
		
	}
}
