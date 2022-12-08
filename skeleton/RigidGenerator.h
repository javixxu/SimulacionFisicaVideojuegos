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
		}
	};
	void setPhysx(PxPhysics* gP) { gPhysics = gP; };
	void addRigidForceRegistry(RigidForceRegistry* prf) { rfr = prf; };

	RigidSolid* Clone(RigidSolid* rg ) {
		RigidSolid* nuevo = new RigidSolid();
		nuevo->timeAlive = nuevo->maxTimeAlive = rg->maxTimeAlive;
		auto pos = rg->solidType->getGlobalPose().p;
		auto y = static_cast<PxRigidDynamic*>(rg->solidType);
		PxRigidActor* new_solid;
		auto typee = rg->item->shape->getGeometryType();
		PxShape* shape;
		switch (typee)
		{
		case physx::PxGeometryType::eSPHERE:
			if (type == DYNAMIC) {
				auto x = gPhysics->createRigidDynamic(PxTransform(pos));
				x->setLinearVelocity(y->getLinearVelocity());
				x->setAngularVelocity(y->getAngularVelocity());
				x->setMass(y->getMass());
				new_solid = x;
			}
			else {
				new_solid= gPhysics->createRigidDynamic(PxTransform(pos));
			}
			/*shape = PxRigidActorExt::createExclusiveShape(*new_solid, PxSphereGeometry(5.0),
				*rg->item->shape->getMaterialFromInternalFaceIndex(1.0));*/
			shape = CreateShape(PxSphereGeometry(2.0));
			break;
		/*case physx::PxGeometryType::ePLANE:
			break;
		case physx::PxGeometryType::eCAPSULE:
			break;
		case physx::PxGeometryType::eBOX:
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
			break;*/
	/*	default:
			break;*/
		}
		//guardo las cosas
		new_solid->attachShape(*shape);
		//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
		nuevo->item = new RenderItem(shape, new_solid, rg->item->color);
		gScene->addActor(*new_solid);
		nuevo->solidType = new_solid;

		if (y != nullptr) { //Le meto las fuerzas
			auto aux2 = static_cast<PxRigidDynamic*>(nuevo->solidType);
			for (int i = 0; i < _model->forcesNames.size(); i++) {
				nuevo->forcesNames.push_back(_model->forcesNames[i]);
				rfr->addRegistry(rfr->getForceGeneration(_model->forcesNames[i]), aux2);				
			}
		}

		return nuevo;
	};
};

