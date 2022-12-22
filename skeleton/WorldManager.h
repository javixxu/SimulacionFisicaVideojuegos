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
	vector<PxMaterial*> list_materials;
	RigidForceRegistry* rfr;

	RigidSolid* golfBall;

	int numMax=120;
	int currentNum;
public:
	WorldManager(PxScene* gScene, PxPhysics* gPhysics);
	~WorldManager();
	void eliminarCuerpo(RigidSolid* s);
	void vaciarEntidades();
	void vaciarGeneradores();
	void update(double duration);
	void setNewMaterial(PxMaterial* x);
	
	PxRigidStatic* addStaticBox(Vector3 pos, Vector3 tam,Vector4 color,PxQuat q=PxQuat(0.0,0.0,0.0,1.0),int idMaterial=0);
	PxRigidDynamic* addDynamicBall(Vector3 pos, double tam, Vector3 vel, Vector4 color);

	inline bool CanGenerateObject() { return numMax > currentNum; };
	inline void updateNumObjects(int num) { currentNum += num; };
	inline PxScene* getScene() { return gScene; };

	shared_ptr<RigidGenerator> getRigidGenerator(string name);
	shared_ptr<ForceGenerator> getForceGenerator(string name);
	PxMaterial* getMaterial(int id);

	void systemOne();
	void systemWind(string name, Vector3 pos, Vector3 dir, Vector3 tam, float range);

	void CreateGolfBall(Vector3 pos, double tam, Vector3 vel, Vector4 color);
	inline PxRigidDynamic* getRigidGolfBall() { return static_cast<PxRigidDynamic*>(golfBall->solidType); };

	void createCorridorVertical(Vector3 pos, Vector3 tam,int idMaterial=0,Vector4 colorSuelo= { 0.2,1.0,0.2,1.0 });
	void createCorridorHorizontal(Vector3 pos, Vector3 tam, int idMaterial=0, Vector4 colorSuelo = { 0.2,1.0,0.2,1.0 });
	void inicioCorridor(Vector3 pos, Vector3 tam,bool front,int idMaterial = 0, Vector4 colorSuelo = { 0.2,1.0,0.2,1.0 });
	void curvaDerechaCorridor(Vector3 pos, Vector3 tam,bool front, int idMaterial = 0, Vector4 colorSuelo = { 0.2,1.0,0.2,1.0 });
	void curvaIzquierdaCorridor(Vector3 pos, Vector3 tam, bool front, int idMaterial = 0, Vector4 colorSuelo = { 0.2,1.0,0.2,1.0 });
	void rampa(Vector3 pos, Vector3 tam,PxQuat q, int idMaterial = 0, Vector4 colorSuelo = { 0.2,1.0,0.2,1.0 });
	void sueloLiso(Vector3 pos, Vector3 tam, int idMaterial = 0, Vector4 colorSuelo = { 0.2,1.0,0.2,1.0 });

	void sueloBaseCollision(Vector3 pos, Vector3 tam, char* Name);
	void hoyo(char* name,Vector3 pos, Vector3 tam);
	void crearObjectoVacio(Vector3 pos, Vector3 tam, string size);

	void muelleDinamico(string name, Vector3 pos, double k, double resting, bool z=false);

};

