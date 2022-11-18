#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos) :SpringForceGenerator(nullptr,k, resting)
{
    other = new Particle(anchor_pos, { 0,0,0 }, {0,0,0},0.999,2.0,Particle::UNUSED,{ 1.0, 1.0, 0.0, 1.0 });
    setName("AnchoredSpring");
}
