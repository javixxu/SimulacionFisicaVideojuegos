#include "Particle.h"

class Proyectil :public Particle {
private: TYPE TYPE;
public:
	Proyectil(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double size);
	~Proyectil();
};
