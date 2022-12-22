#pragma once
#include <list>
#include <Time.h>

#include "Particle.h"
#include "ParticleGenerator.h"
#include "Firework.h"
#include "ParticleForceRegistry.h"

using namespace std;

class ParticleSystem{

	list<Particle*> list_particles;
	list<Particle*> list_linea;
	list<shared_ptr<ParticleGenerator>>list_generator;
	vector<Firework*> fireworks_pool;
	Vector3 gravity = Vector3(0.0, -9.8, 0.0);
	ParticleForceRegistry* pForceRegistry;
	list<shared_ptr<ForceGenerator>> list_forces;
public:
	ParticleSystem();
	~ParticleSystem();
	void update(double t);
	shared_ptr<ParticleGenerator> getParticleGenerator(string name);
	shared_ptr<ForceGenerator> getForceGenerator(string name);
	void vaciarGeneradores() { list_generator.clear(); };
	void vaciarEntidades();
	void generateFireworkSystem();//no creado
	void generateFogSystem(string name, Vector3 pos , Vector3 tam, Vector3 dir);
	void generateFlamesSystem();
	void generateHosepipeSystem();
	void generateCircleSystem();
	void generateRocketSystem(string name,Vector3 pos);
	void generateSpringSystem();
	void generateWindSystem(string name,Vector3 pos, Vector3 tam,Vector3 dir);
	void generateWhirlSystem();
	void generateExplosionSystem();
	void generateElasticBandSystem();
	void Slinky();
	void generaflotacion(string name,Vector3 position,Vector3 tam);

	void updateLineaDisparo(Vector3 pos,Vector3 dir,bool desactivar);
	void generarLineaDeDisparo(Vector3 pos, Vector3 dir);

	void increaseDesTip(Vector3 increase);
	void increaseHeightFloatSystem(float x);
	void increaseVolumeFloatSystem(float x);

	void shootFirework(Firework::FIREWORK_TYPE type);
	void onParticleDeath(Particle* pt);
	void explosion(bool activee);
};

