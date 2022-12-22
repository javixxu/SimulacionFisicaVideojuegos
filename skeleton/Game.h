#pragma once
#include <list>
#include <memory>
#include "PxPhysics.h"
#include "PxScene.h"
#include "RenderUtils.hpp"
#include "ParticleSystem.h"
#include "WorldManager.h"
#include <string>
#include "define_materials.h"
using namespace std;
using namespace physx;

class Game{

	WorldManager* worldManager;
	ParticleSystem* particleSystem;
	Camera* myCamera;
	PxRigidDynamic* golfBall;
	PxVec3 fuerzaGolfBall= Vector3(0);
	Vector3 safePosition;
	Vector3 safeDir;
	bool sumarFuerza = false;
	const Vector3 maxForce = { 250,0,250 };
	string nextLevel = "null";

	float timeAnimation = 7.0;
	bool controlActive = true;
public: 
	Game(PxScene* gScene, PxPhysics* gPhysics,Camera* camera);
	~Game();
	void update(float duration);
	void seeMouseInput(int button, int state, int x, int y);
	void keyPress(unsigned char key);
	PxRigidDynamic* getBallGolf() { return golfBall; };
	void createLevel1();
	void createLevel2();
	bool canMove();
	void keepSafePosBall(Vector3 safe);
	void retakeBall();
	void collisions(PxActor* actor1,PxActor* actor2);
	void selectLevel(string name);
	void limpiarNivel();
	inline bool getControlsActive() { return controlActive; };
};

