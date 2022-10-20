#include "Firework.h"
#include<conio.h>
#include<math.h>

Firework::Firework() :Particle() {
};
Firework::Firework(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double Mass, FIREWORK_TYPE type_, Vector4 COLOR )
	:Particle(Pos,Vel,acceleration,Damping,Mass,FIREWORK,COLOR),type(type_) {

}
Firework* Firework::clone() const {
	Firework* newRK=new Firework(pose.p,vel,acc,damping,mass,type);
	newRK->setTimeAlive(initialTimeAlive);
	return newRK;
}

void Firework::update(double t){
	integrate(t);
}

void Firework::explode(list<Particle*> list){
	switch (type)
	{
	case Firework::BASIC:
		/*double angle = 90.0; double radio = 50;
		for (int i = 0; i < 20; i++){
			Vector3 pos(50 + ,0,0);
			angle += 360.0 / 20;
		}*/
		shared_ptr<Particle> x(clone());		
		x->setVelocity(Vector3(10, 0, 0));
		x->setAcceleration(Vector3()); x->setTimeAlive(initialTimeAlive / 2);
		gens.push_back(x);

		shared_ptr<Particle> y(clone());
		y->setVelocity(Vector3(-10, 0, 0));
		y->setAcceleration(Vector3()); y->setTimeAlive(initialTimeAlive / 2);
		gens.push_back(y);
	break;
	
	}
}
