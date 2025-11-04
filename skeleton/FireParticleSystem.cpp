#include "FireParticleSystem.h"
#include "UniformalGen.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"

FireParticleSystem::FireParticleSystem(Particula* p, PxPhysics* gPhysics)
{
    // crear generador
    UniformalGen* gausGen = new UniformalGen(30, 0.5, p, gPhysics);
    
    // Ajustar las desviaciones del fuego (esto antes estaba dentro de UniformalGen)
    gausGen->setModelP(p);
    gausGen->setParticulas(10);
    gausGen->setProbGen(0.8);

    // pequeñas variaciones de posición al generar
    gausGen->setInitialPos(Vector3(0.5, 0.0, 0.5));

    // desviación de velocidad -> turbulencia lateral y ascenso
    gausGen->setVel(Vector3(2.5, 8.0, 2.5));

    // duración con leve variación
    gausGen->setDuration(1.0);


    _generators.push_back(gausGen);

    // generar partículas y moverlas a _particles
    if (!_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
            _particles.splice(_particles.end(), newParticles);
    }

    //crear registro fuerzas
    _registry = new ParticleForceRegistry();

    //añadir fuerza gravitatoria
    // Crear dos generadores de gravedad diferentes
    gravityEarth = new GravityForceGenerator();

    // Registrar fuerzas
    for (auto p : _particles)
    {
        _registry->add(p, gravityEarth);
    }

    //viento
    windForce = new WindForceGenerator(Vector3(6.0, 6.0, 0.0), 0.08, 0);

    // Asignar a partículas
    for (auto p : _particles) {
        _registry->add(p, windForce);
    }

    //Fuerza explosion
    whirlWindForce = new WhirlwindForceGenerator(Vector3(35, 35, 35), 2, 1, 2);

    // Asignar a partículas
    for (auto p : _particles) {
        _registry->add(p, whirlWindForce);
    }

    //como no quiero gravedad o viento... 
    gravityEarth->setActive(false);
    windForce->setActive(false);
}

FireParticleSystem::~FireParticleSystem()
{
}

void FireParticleSystem::update(double t)
{
    //actualizar fuerzas
    _registry->updateForces(t);

    // Generar nuevas partículas cada frame
    if (!_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
        {
            // Registrar cada nueva partícula en el registro de fuerzas
            for (auto p : newParticles) {
                _registry->add(p, gravityEarth);
                _registry->add(p, windForce);
                _registry->add(p, whirlWindForce);
            }
            _particles.splice(_particles.end(), newParticles);
        }

    }

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
