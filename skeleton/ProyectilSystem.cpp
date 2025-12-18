#include "ProyectilSystem.h"
#include "Proyectil.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"

ProyectilSystem::ProyectilSystem()
{
	//En este caso no se generan desde un generador, sino que se llama al createProyectil

    //crear registro fuerzas
    _registry = new ParticleForceRegistry();

    //añadir fuerza gravitatoria
    gravityEarth = new GravityForceGenerator();

    // Registrar fuerzas
    for (auto p : _particles)
    {
        _registry->add(p, gravityEarth);
    }

    //inicializar a null los que no queremos
    windForce = nullptr;
    whirlWindForce = nullptr;
}

ProyectilSystem::~ProyectilSystem()
{
}

void ProyectilSystem::update(double t)
{
    //actualizar fuerzas
    _registry->updateForces(t);

    // Actualizar todas las existentes
    for (auto p : _particles) {
        p->integrate(t);
    }

    _particles.remove_if([this](Particula* p) {
        if (p->getTimeVida() <= 0.0) {
            if (p->getRenderItem()) {
                DeregisterRenderItem(p->getRenderItem());
                delete p->getRenderItem();
            }
            _registry->remove(p);
            delete p;
            return true;
        }
        return false;
        });

}

void ProyectilSystem::createProyectil(Vector4 color, double masaR, double velR, double velS, physx::PxShape* esferaShape)
{
	Proyectil* p = new Proyectil(color, masaR, velR, velS, esferaShape);
    _registry->add(p->getParticle(), gravityEarth);
    _particles.push_back(p->getParticle());
}
