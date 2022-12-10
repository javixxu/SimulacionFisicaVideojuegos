#pragma once
#include <string>
#include <list>
#include "PxPhysics.h"
#include "RigidForceRegistry.h"
#include "WorldManager.h"
#include "PxScene.h"
#include "RenderUtils.hpp"

using namespace std;
using namespace physx;

enum typeRigid { STATIC, DYNAMIC };
struct RigidSolid;

class RigidGenerator {
protected:
	std::string name;
	RigidForceRegistry* rfr;
	RigidSolid* _model=nullptr;
	WorldManager* mng;
	PxScene* gScene;
	PxPhysics* gPhysics;
	PxMaterial* mat=nullptr;

	Vector3 mean_pos, mean_vel;
	int num_particles=20;
	bool active=true;
	bool rgbRandom;
	double mass;
	
public:
	typeRigid type;
	virtual void generateRigid(list<RigidSolid*>& l) = 0;
	void setName(string Name) { name = Name; }
	string getName() { return name; }
	void setBody(RigidSolid* _Model) {
		if (_model != nullptr) { 
			DeregisterRenderItem(_model->item);
			delete _model; 
		}
		_model = _Model;
	}
	inline bool isActive() { return active; }

	inline void setRegistry(RigidForceRegistry* rfrr) {
		rfr = rfrr;
	};

	void setNumGenerator(int x) { 
		num_particles = x; 
	};

	inline void changeActive() {
		active = !active;
		if (_model != nullptr) {
			Vector3 pos = _model->solidType->getGlobalPose().p;
			if (!active)pos.y += 4000;
			else pos.y -= 4000;
			_model->solidType->setGlobalPose(PxTransform(pos));
		}
	};
	void setPhysx(PxPhysics* gP) { gPhysics = gP; };
	void setMaterial(PxMaterial* matt) { mat = matt; };
	void addRigidForceRegistry(RigidForceRegistry* prf) { rfr = prf; };

	RigidSolid* Clone(RigidSolid* rg ) {
		RigidSolid* nuevo = new RigidSolid();
		nuevo->timeAlive = nuevo->maxTimeAlive = rg->maxTimeAlive;
		auto pos = rg->solidType->getGlobalPose().p;
		auto y = static_cast<PxRigidDynamic*>(rg->solidType);
		PxRigidActor* new_solid;
		auto typee = rg->item->shape->getGeometryType();

		if (type == DYNAMIC) {
			auto x = gPhysics->createRigidDynamic(PxTransform(pos));
			x->setLinearVelocity(y->getLinearVelocity());
			x->setAngularVelocity(y->getAngularVelocity());
			x->setLinearDamping(y->getLinearDamping());
			x->setMassSpaceInertiaTensor(y->getMassSpaceInertiaTensor());
			new_solid = x;
		}
		else new_solid = gPhysics->createRigidDynamic(PxTransform(pos));
		
		PxShape* shape;

		switch (typee)
		{
		case physx::PxGeometryType::eSPHERE:
			shape = CreateShape(PxSphereGeometry(2.0),mat);
			break;
		case physx::PxGeometryType::eBOX:			
			shape = CreateShape(PxBoxGeometry(Vector3(3.5)),mat);
			break;
		/*case physx::PxGeometryType::ePLANE:
			break;
		case physx::PxGeometryType::eCAPSULE:
			break;
		case physx::PxGeometryType::eCONVEXMESH:
			break;
		case physx::PxGeometryType::eTRIANGLEMESH:
			break;
		case physx::PxGeometryType::eHEIGHTFIELD:
			break;
		case physx::PxGeometryType::eGEOMETRY_COUNT:
			break;
		case physx::PxGeometryType::eINVALID:
			break;
		default:
			break;*/
		}
		//guardo las cosas
		new_solid->attachShape(*shape);
		//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
		nuevo->item = new RenderItem(shape, new_solid, rg->item->color);
		gScene->addActor(*new_solid);
		nuevo->solidType = new_solid;
		
		if (type == DYNAMIC) { //Le meto las fuerzas
			PxRigidBodyExt::setMassAndUpdateInertia(*(static_cast<PxRigidDynamic*>(nuevo->solidType)), 3.0);
			auto aux2 = static_cast<PxRigidDynamic*>(nuevo->solidType);
			for (int i = 0; i < _model->forcesNames.size(); i++) {
				nuevo->forcesNames.push_back(_model->forcesNames[i]);
				rfr->addRegistry(rfr->getForceGeneration(_model->forcesNames[i]), aux2);				
			}
		}

		return nuevo;
	};
};

