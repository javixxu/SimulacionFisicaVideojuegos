#include "FloatBounceForce.h"

FloatBounceForce::FloatBounceForce(float h, float v, float d)
    : height(h), volume(v), liquid_density(d)
{
    lqparticle = new Particle({ 0, 0, 0 }, {}, {},0.999, 50);
    lqparticle->setPosition({ 0, 0, 0 });
}

FloatBounceForce::~FloatBounceForce(){
    delete lqparticle;
}

void FloatBounceForce::updateForce(Particle* p, double t){
    float h = p->getPosition().y;
    float h0 = lqparticle->getPosition().y;

    Vector3 f(0, 0, 0);
    float immersed = 0.0;

    if (h - h0 > height * 0.5)
    {
        immersed = 0.0;
    }
    else if (h0 - h > height * 0.5) {
        immersed = 1.0;
    }
    else
    {
        immersed = (h0 - h) / height + 0.5;
    }

    f.y = liquid_density * volume * immersed * 9.8;

    p->addForce(f);
}
