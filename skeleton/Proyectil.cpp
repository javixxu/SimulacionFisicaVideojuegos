#include "Proyectil.h"

Proyectil::Proyectil():Particle(Vector3(0),Vector3(0),Particle::PROYECTIL) {}
Proyectil::Proyectil(Vector3 Pos, Vector3 Vel):Particle( Pos, Vel, TYPE::PROYECTIL) {}
Proyectil::Proyectil(Vector3 Pos, Vector3 Vel, Vector4 COLOR) :Particle( Pos, Vel,COLOR, TYPE::PROYECTIL) {}
Proyectil::Proyectil(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double Mass):
	Particle( Pos,  Vel, acceleration, Damping, Mass, TYPE::PROYECTIL) {}