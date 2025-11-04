#include "ConfettiParticleSystem.h"
#include "UniformalGen.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"

ConfettiParticleSystem::ConfettiParticleSystem(Particula* p, PxPhysics* gPhysics)
{
    // crear generador
    UniformalGen* confettiGen = new UniformalGen(30, 0.5, p, gPhysics);

    // Ajustar las desviaciones del fuego (esto antes estaba dentro de UniformalGen)
    //confettiGen->setModelP(p);
    _modelP = p;
    confettiGen->setParticulas(5);
    confettiGen->setProbGen(0.8);

    // Randomización de dirección
    double angulo = ((double)rand() / RAND_MAX) * 2.0 * 3.14159265359;
    Vector3 dir(cos(angulo), 0.5, sin(angulo)); // 0.5 para dar un impulso vertical medio
    dir.normalize();

    // Escalar por la "potencia" deseada
    double potencia = 15.0; // aumenta este valor para que salga más rápido
    Vector3 vel = dir * potencia;

    // Asignar a las desviaciones
    confettiGen->setVel(vel);

    confettiGen->setInitialPos(Vector3(0.0, 0.0, 0.0));
    confettiGen->setDuration(1.0);
    confettiGen->setDesColor(Vector4(0.3f, 0.7f, 0.8f, 1.0f));

    _generators.push_back(confettiGen);

    // generar partículas y moverlas a _particles
    if (!_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
            _particles.splice(_particles.end(), newParticles);
    }

    // Registro de fuerzas
    _registry = new ParticleForceRegistry();
    gravityEarth = new GravityForceGenerator();

    // Registrar fuerza de gravedad a todas las partículas
    for (auto p : _particles) {
        _registry->add(p, gravityEarth);
    }
}

ConfettiParticleSystem::~ConfettiParticleSystem()
{
}

void ConfettiParticleSystem::update(double t)
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
