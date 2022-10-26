#include "Proyectil.h"

Proyectil::Proyectil(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double size):
	Particle( Pos,  Vel, acceleration, Damping, size, TYPE::PROYECTIL) {}