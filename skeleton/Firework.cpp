#include "Firework.h"
#include<conio.h>
#include<math.h>
#include "RocketGenerator.h"

Firework::Firework() :Particle() {
};

Firework::Firework(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double size, FIREWORK_TYPE type_, Vector4 COLOR,int numGenn )
	:Particle(Pos,Vel,acceleration,Damping,size,FIREWORK,COLOR),type(type_),numGen(numGenn) {

}

Firework* Firework::clone() const {
	Firework* newRK=new Firework(pose.p,vel,acc,damping,size_,type, color);
	newRK->setTimeAlive(initialTimeAlive); newRK->setNumGen(numGen);
	for (auto it : gens)newRK->addGenerator(it);
	return newRK;
}

void Firework::explode(list<Particle*>& list){
	switch (type)
	{
	case Firework::BASIC:		
		for (auto it:gens){			
			auto pt = clone(); 
			pt->setTimeAlive(getInitialTimeAlive() / 2);
			pt->deleteGens();
			pt->setAcceleration(Vector3(0, -10, 0));
			pt->setVelocity(pt->getVelocity());
			pt->setColor(Vector4(1.0, 0.0, 0.0, 1.0));
			(*it).setNumGenerator(20);
			(*it).setParticle(pt);
			(*it).generateParticles(list);
			 pt->setPosition(Vector3(10000, 100000, 0));
		}
	break;
	case Firework::LINEAR:
		for (auto it : gens) {
			auto pt = clone();
			pt->setNumGen(numGen + 1);
			
			if (pt->getNumGen() >= 1) { 
				pt->type = BASIC; 				
			}
			pt->setTimeAlive(getInitialTimeAlive() / 2);
			pt->setAcceleration(Vector3(1, -10/(numGen+1), 1));
			pt->setVelocity(pt->getVelocity());

			

			(*it).setParticle(pt);
			(*it).setNumGenerator(20);
			(*it).generateParticles(list);
			pt->setPosition(Vector3(10000, 100000, 0));
		}
		break;
	case Firework::CIRCULAR:
		for (auto it : gens) {
			auto pt = clone(); 
			pt->setNumGen(numGen + 1);
			
			if (numGen+1 >= 2)
				pt->type = NONE;
			pt->setTimeAlive(getInitialTimeAlive());
			auto r = rand() % 255 + 0;
			auto g = rand() % 255 + 0;
			auto b = rand() % 255 + 0;
			pt->setColor(Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0));
			pt->setAcceleration(Vector3(0, -10/(numGen+1), 0));
			pt->setVelocity(pt->getVelocity());
			

			(*it).setNumGenerator(20);
			(*it).setParticle(pt);
			(*it).generateParticles(list);
			pt->setPosition(Vector3(10000, 100000, 0));
		}
	break;
	default:
		break;
	}
}
