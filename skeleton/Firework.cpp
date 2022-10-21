#include "Firework.h"
Firework::Firework() :Particle() {
};
Firework::Firework(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double Mass, TYPE Type, Vector4 COLOR )
	:Particle(Pos,Vel,acceleration,Damping,Mass,Type,COLOR) {

}
Firework* Firework::clone() const {

}