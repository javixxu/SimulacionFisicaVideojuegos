#pragma once
#include <list>
#include <memory>
#include "PxPhysics.h"
#include "PxScene.h"
#include "RenderUtils.hpp"
#include "RigidForceRegistry.h"
#include "ExplosionForceGenerator.h"
using namespace std;
using namespace physx;
class RigidGenerator;

struct RigidSolid {
	PxRigidActor* solidType;
	RenderItem* item;
	vector<string> forcesNames;
	double maxTimeAlive;
	double timeAlive=-1.0;
};

class WorldManager{
protected:
	PxScene* gScene;
	PxPhysics* gPhysics;
	list<RigidSolid*> list_static;
	list<RigidSolid*> list_dynamic;
	list<shared_ptr<ForceGenerator>> list_forces;
	list<shared_ptr<RigidGenerator>> list_generators;
	RigidForceRegistry* rfr;
	int numMax=25;
	int currentNum;
public:
	WorldManager(PxScene* gScene, PxPhysics* gPhysics);
	~WorldManager();
	void update(double duration);
	void eliminarCuerpo(RigidSolid* s);
	void setMaterialToObject(PxRigidActor*x,Vector3 mat);
	void addStaticBox(Vector3 pos, Vector3 tam,Vector4 color);
	void addDynamicBall(Vector3 pos, double tam, Vector3 vel, Vector4 color);
	inline bool CanGenerateObject() { return numMax > currentNum; };
	inline void updateNumObjects(int num) { currentNum += num; };
	inline PxScene* getScene() { return gScene; };
	shared_ptr<RigidGenerator> getRigidGenerator(string name);
	shared_ptr<ForceGenerator> getForceGenerator(string name);
	void systemOne();
	void systemWind();
};

