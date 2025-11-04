#include "WhirlwindForceGenerator.h"

WhirlwindForceGenerator::WhirlwindForceGenerator(Vector3 o, double k1, double s, double r): origin(o), k1(k1), strength(s), radius(r)
{
}

void WhirlwindForceGenerator::update(Particula* particle, double t)
{
    if (active)
    {
        //Setear velocidad del viento
        Vector3 tangential((-(particle->getPos().z - origin.z)), 0.0, (particle->getPos().x - origin.x));
        tangential = tangential * k1;

        // Escalado de fuerza
        double forceMag = strength * ((particle->getPos() - origin).magnitude() / radius);

        // limitar un max
        forceMag = std::clamp(forceMag, 0.0, 50.0);

        // aplicar fuerza
        Vector3 totalForce = tangential * forceMag;
        particle->addForce(totalForce);
    }
    
}
