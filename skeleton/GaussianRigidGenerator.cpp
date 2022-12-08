#include "GaussianRigidGenerator.h"
#include <math.h>
#include <list>
#include <Time.h>
#include <random>

GaussianRigidGenerator::GaussianRigidGenerator(WorldManager* mngg,typeRigid typee ,Vector3 pos, Vector3 vel, double desvTiempo, double masss, bool rgb)
{
	mng = mngg; type = typee;
	mean_pos = pos;mean_vel = vel;
	rgbRandom = rgb; mass = masss;
	gScene = mng->getScene();
}

GaussianRigidGenerator::~GaussianRigidGenerator(){
	setBody(nullptr);
	mng = nullptr;
	rfr = nullptr;
}

void GaussianRigidGenerator::generateRigid(list<RigidSolid*>& l){
	Vector3 newpos, newvel,posmolde=_model->solidType->getGlobalPose().p;
	auto y = static_cast<PxRigidDynamic*>(_model->solidType);
	for (int i = 0; i < num_particles; i++) {
		if (!mng->CanGenerateObject())return;
		
		if (rgbRandom) {
			auto r = rand() % 255 + 0;
			auto g = rand() % 255 + 0;
			auto b = rand() % 255 + 0;
			_model->item->color = Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0);
		}
		newpos = posmolde + Vector3(mean_pos.x * d(gnd), mean_pos.y * d(gnd), mean_pos.z * d(gnd));
		RigidSolid* nueva = Clone(_model);
		nueva->solidType->setGlobalPose(PxTransform(newpos));
		if (type==DYNAMIC) {
			newvel = y->getLinearVelocity() + 
				Vector3(mean_vel.x * d(gnd), mean_vel.y * d(gnd), mean_vel.z * d(gnd));
			auto aux = static_cast<PxRigidDynamic*>(nueva->solidType);
			aux->setMass(y->getMass() + abs(d(gnd) * mass));
			aux->setLinearVelocity(newvel);
		}
		//nueva->timeAlive = nueva->maxTimeAlive =_model->maxTimeAlive+ devTip_t * d(gnd); 
		//cout << nueva->maxTimeAlive << "\n";
		
		mng->updateNumObjects(1);
		l.push_back(nueva);
	}
}

void GaussianRigidGenerator::increaseDesTipVel(Vector3 increase){
	mean_vel += increase;
	if (mean_vel.x < 0) mean_vel.x = 0;
	if (mean_vel.y < 0) mean_vel.y = 0;
	if (mean_vel.z < 0) mean_vel.z = 0;
}
