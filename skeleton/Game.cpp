#include "Game.h"
#include <glut.h>

Game::Game(PxScene* gScene, PxPhysics* gPhysics, Camera* camera) {
	particleSystem = new ParticleSystem();
	worldManager = new WorldManager(gScene, gPhysics);
	myCamera = camera;
	fuerzaGolfBall = Vector3(0);
	sumarFuerza = false;
	myCamera->setGlofBall(worldManager->getRigidGolfBall(),this);
	myCamera->update();
	golfBall = static_cast<PxRigidDynamic*>(worldManager->getRigidGolfBall());
	keepSafePosBall(golfBall->getGlobalPose().p);
	particleSystem->generarLineaDeDisparo(golfBall->getGlobalPose().p, myCamera->getDir());
	worldManager->setNewMaterial(gPhysics->createMaterial(0.3f, .1f, 1.0f));//HIELO ID=1;
	worldManager->setNewMaterial(gPhysics->createMaterial(0.9f, 0.9f, 0.5f));//Pehgajoso ID=2;
	createLevel1();	
}

Game::~Game(){
	delete particleSystem;
	delete worldManager;
}

void Game::update(float duration){
	if (nextLevel != "null") {
		timeAnimation -= duration;
		if (timeAnimation <= 0.0) {
			limpiarNivel();
			selectLevel(nextLevel);
		}
	}
	myCamera->update();	
	worldManager->update(duration);
	particleSystem->updateLineaDisparo(golfBall->getGlobalPose().p,myCamera->getDir(), canMove());
	particleSystem->update(duration);
	if (sumarFuerza&&controlActive) {
		fuerzaGolfBall += Vector3(150*duration, 0, 150*duration);
	}
}

void Game::seeMouseInput(int button, int state, int x, int y){
	if (!canMove())return;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//std::cout << "sumandooo" << "\n";
		sumarFuerza = true;
		keepSafePosBall(golfBall->getGlobalPose().p);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		//std::cout << "fin" << "\n";	
		if (!sumarFuerza)return;
		fuerzaGolfBall.x *= myCamera->getDir().x; 
		fuerzaGolfBall.z *= myCamera->getDir().z;
		//cout << "x: " << fuerzaGolfBall.x << " " << "y: " << fuerzaGolfBall.y << " " << "z: " << fuerzaGolfBall.z << "\n";
		if (maxForce.x < fuerzaGolfBall.x || maxForce.z < fuerzaGolfBall.z)fuerzaGolfBall = maxForce;
		//cout << "x: " << fuerzaGolfBall.x << " " << "y: " << fuerzaGolfBall.y << " " << "z: " << fuerzaGolfBall.z << "\n";
		golfBall->setLinearVelocity(fuerzaGolfBall);
		fuerzaGolfBall = Vector3(0);
		sumarFuerza = false;
	}
}

void Game::keyPress(unsigned char key){
	if (!controlActive)return;
	switch (toupper(key)) {
	case'F':
		if (sumarFuerza) {
			sumarFuerza = false;
			fuerzaGolfBall = Vector3(0);
		}
		break;
	case 'E':
		retakeBall();
		break;
	case 'X':
		golfBall->setGlobalPose(PxTransform(Vector3(20.0, 10.0, -310)));
		break;
	case ' ':
		myCamera->setFollow(!myCamera->getFollow());
		break;
	}
}

void Game::createLevel1(){
	worldManager->sueloBaseCollision(Vector3(0, -1, 0), Vector3(500, 0, 500), "SUELO-BASE");
	worldManager->inicioCorridor({ 0,0,0 }, { 50,3,25 }, false);
	worldManager->curvaIzquierdaCorridor({ 65,0,0 }, { 25,3,25 }, true);
	worldManager->curvaDerechaCorridor({ 65,0,25 }, { 25,3,25 }, false);
	worldManager->createCorridorVertical({ 107,0,25 }, { 25,3,25 });
	worldManager->sueloLiso({ 155,0,25 }, { 25,3,25 });
	worldManager->muelleDinamico("muelle1", { 125,40,25 }, 10, 10);
	worldManager->muelleDinamico("muelle2", { 120,40,25 }, 9, 10);
	worldManager->muelleDinamico("muelle3", { 115,40,25 }, 8, 10);
	worldManager->muelleDinamico("muelle4", { 110,40,25 }, 9, 10);

	worldManager->hoyo("hoyo1", {150,0.0,25}, {5,0.2,5});
	particleSystem->generateRocketSystem("FireworkFinal", { 147.5,15.0,25 });
	keepSafePosBall({ 0,5,0 }); retakeBall();
	golfBall->wakeUp();	timeAnimation = 7.0;
	myCamera->setDir({ 0.954597 , -0.296454 , -0.029329 });
}

void Game::createLevel2(){
	worldManager->sueloBaseCollision(Vector3(0, -1, 0), Vector3(500, 0, 500), "SUELO-BASE");
	worldManager->sueloLiso({ 150,0,25 }, { 25,3,25 });
	worldManager->hoyo("hoyo2", { 20.0,10.0,-320 }, { 5,0.2,5 });
	worldManager->sueloLiso({ 20.0,10.0,-315 }, { 13,1,15 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->createCorridorVertical({ 199.5,0,25 }, { 25,3,25 },MATERIAL_HIELO,COLOR_HIELO);
	worldManager->curvaDerechaCorridor({ 243.25,0,25 }, { 25,3,25 }, true);
	worldManager->rampa({ 243.25,5,0 }, { 20,3,20 }, PxQuat(170.0, { 1.0, 0.0, 0.0 }));
	worldManager->rampa({ 243.25,5,-90 }, { 20,3,20 }, PxQuat(-170.0, { 1.0, 0.0, 0.0 }));
	worldManager->createCorridorHorizontal({ 243.25,0,-125 }, { 25,3,25 }, MATERIAL_HIELO,COLOR_HIELO);
	worldManager->curvaIzquierdaCorridor({ 237,0,-162.5 }, { 25,3,25 }, true, MATERIAL_HIELO, COLOR_HIELO);
	particleSystem->generaflotacion("flotacion1", {243.25,1,-45}, Vector3(50, 0, 17));
	worldManager->curvaDerechaCorridor({ 212.0,0,-137.5 }, { 25,3,25 }, true);
	
	worldManager->createCorridorHorizontal({ 205.0,0,-175.0 }, { 25,3,25 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->createCorridorHorizontal({ 205.0,0,-225.0 }, { 25,3,25 }, MATERIAL_PEGAJOSO, COLOR_PEGAJOSO);
	worldManager->createCorridorHorizontal({ 205.0,0,-275.0 }, { 25,3,25 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->curvaIzquierdaCorridor({ 198.75,0,-312.5 }, { 25,3,25 }, true);
	worldManager->createCorridorVertical({ 168.0,0,-312.5 }, { 25,3,25 });
	worldManager->createCorridorVertical({ 118.0,0,-312.5 }, { 25,3,25 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->createCorridorVertical({ 68.0,0,-312.5 }, { 25,3,25 });
	worldManager->rampa({ 54.0,4.25,-312.5 }, { 22,0,10 }, PxQuat(50.0, { 0,0,1 }));
	worldManager->muelleDinamico("muelle1", { 68.0,35,-312.5 }, 8, 11);
	worldManager->muelleDinamico("muelle2", { 78.0,40,-312.5 }, 10, 10);
	worldManager->muelleDinamico("muelle3", { 88.0,35,-312.5 }, 8, 11);
	worldManager->muelleDinamico("muelle4", { 98.0,40,-312.5 }, 10, 10);
	worldManager->muelleDinamico("muelle5", {108.0,35,-312.5 }, 8, 11);
	worldManager->systemWind("WindSystem1", { 270.0,45,-235.0 }, { -15,0,0 }, { 0,1,35 }, 200);

	worldManager->createCorridorVertical({ 168.5,0,-137.5 }, { 25,3,25 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->createCorridorVertical({ 118.5,0,-137.5 }, { 25,3,25 },MATERIAL_PEGAJOSO, COLOR_PEGAJOSO);
	worldManager->createCorridorVertical({ 68.5,0,-137.5 }, { 25,3,25 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->curvaDerechaCorridor({ 24.75,0,-137.5 }, { 25,3,25 }, false);
	worldManager->createCorridorHorizontal({ 18.5,0,-175.0 }, { 25,3,25 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->createCorridorHorizontal({ 18.5,0,-225.0 }, { 25,3,25 });
	worldManager->createCorridorHorizontal({ 18.5,0,-275.0 }, { 25,3,25 }, MATERIAL_HIELO, COLOR_HIELO);
	worldManager->rampa({ 18.5,4.35,-279 }, { 10,0,22 }, PxQuat(-50.0, { 1,0,0 }));
	worldManager->muelleDinamico("muelle6", { 18.5,35,-275.0 }, 8, 11, true);
	worldManager->muelleDinamico("muelle7", { 18.5,35,-265.0 }, 10, 10, true);
	worldManager->muelleDinamico("muelle8", { 18.5,35,-255.0 }, 8, 11, true);
	worldManager->muelleDinamico("muelle9", { 18.5,35,-245.0 }, 10, 10, true);
	worldManager->muelleDinamico("muelle10", { 18.5,35,-235.0 }, 8, 11, true);
	worldManager->systemWind("WindSystem2", { 78.5,45,-72.5}, { 0,0,-15 }, { 35,1,0 }, 200);

	particleSystem->generateFogSystem("fog1", Vector3(212.0, 5, -137.5), Vector3(10), Vector3(1.25,2.0,1.25));
	
	particleSystem->generateRocketSystem("FireworkFinal", { 10.0,10.0,-350 });

	golfBall->setLinearVelocity({ 0,5,0 });
	keepSafePosBall({ 150,3,25 });golfBall->wakeUp();
	myCamera->setDir({ 0.92638, -0.375542, -0.0280844 });
	timeAnimation = 12.0;
}

bool Game::canMove(){
	return controlActive && golfBall->getLinearVelocity().magnitude() < 0.18f;
}

void Game::keepSafePosBall(Vector3 ball){
	safePosition = ball;
	safeDir = myCamera->getDir();
}

void Game::retakeBall(){
	golfBall->setLinearVelocity(Vector3(0));
	golfBall->setAngularVelocity(Vector3(0));
	golfBall->setGlobalPose(PxTransform(safePosition));
	myCamera->setDir(safeDir);
}

void Game::collisions(PxActor* actor1, PxActor* actor2){
	if (actor2->getName() == "SUELO-BASE" && actor1->getName() == "GOLFBALL") {
		retakeBall();
	}
	else if ((actor2->getName() == "hoyo1"|| actor2->getName() == "hoyo2") && actor1->getName() == "GOLFBALL") {
		auto hoyo = static_cast<PxRigidStatic*>(actor1);
		if ((golfBall->getLinearVelocity() + golfBall->getGlobalPose().p).magnitude() >= hoyo->getGlobalPose().p.magnitude()) {
			particleSystem->generateRocketSystem("FireworkFinal", Vector3(-1));
			if(actor2->getName() == "hoyo1")myCamera->setDir({ 0.409697,0.912008,0.0197556 });
			else myCamera->setDir({0.0620697 ,0.980941, -0.184127 });
			nextLevel = actor2->getName();
			golfBall->putToSleep();
			controlActive = false;
		}
	}
}

void Game::selectLevel(string name){
	if (name == "hoyo1")  createLevel2(); 
	else  createLevel1(); 
	controlActive = true;
	nextLevel = "null";
}

void Game::limpiarNivel(){
	worldManager->vaciarEntidades();
	worldManager->vaciarGeneradores();
	particleSystem->vaciarEntidades();
	particleSystem->vaciarGeneradores();
}
