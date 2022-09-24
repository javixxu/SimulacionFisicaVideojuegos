#include "Particle.h"

class Proyectil :public Particle {
public:
	Proyectil();
	Proyectil(Vector3 Pos, Vector3 Vel);
	Proyectil(Vector3 Pos, Vector3 Vel, Vector4 COLOR);
	Proyectil(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double Mass);
	~Proyectil();
};
