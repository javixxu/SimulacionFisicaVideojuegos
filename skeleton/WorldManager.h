#pragma once
#include "PxPhysics.h"
#include "PxScene.h"
#include "RenderUtils.hpp"
#include <list>
#include <memory>
using namespace std;
using namespace physx;

struct RigidSolid {
	PxRigidActor* solidType;
	RenderItem* item;
	double currentTime=0.0;
	double timeAlive=-1.0;
};

class WorldManager{
protected:
	PxScene* gScene;
	PxPhysics* gPhysics;
	list<RigidSolid> list_static;
	list<RigidSolid> list_dynamic;
	
public:
	WorldManager(PxScene* gScene, PxPhysics* gPhysics);
	~WorldManager();
	void update(double duration);
	void setMaterialToObject(PxRigidActor*,PxMaterial* material);
	void addWall(Vector3 pos,Vector3 vel,Vector4 color);
	void addBall(Vector3 pos, Vector3 vel, Vector4 color);
};

