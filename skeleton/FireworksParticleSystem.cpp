#include "FireworksParticleSystem.h"
#include "ParticleSystem.h"
#include "GaussianGen.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"

#include <iostream>

FireworksParticleSystem::FireworksParticleSystem(Particula* p, PxPhysics* gPhysics)
{
    contExplosion = 0.5;

    exploded = false;

    // crear generador
    GaussianGen* gausGen = new GaussianGen(1, 0.5, p, gPhysics);
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

    //inicializar a null los que no queremos
    windForce = nullptr;
    whirlWindForce = nullptr;
}

FireworksParticleSystem::~FireworksParticleSystem()
{
}


void FireworksParticleSystem::update(double t)
{
    // Actualizar fuerzas
    _registry->updateForces(t);

    
    // Control del tiempo de la explosión
    if (!exploded)
    {
        contExplosion -= t;

        if (contExplosion <= 0.0)
        {
            contExplosion = 0.0;
            exploded = true;
        }
    }
    else
    {
        if (active)
        {
            const int numGenerations = 4;
            const int particlesPerGen[numGenerations] = { 75, 7, 5, 3 };

            if (generation < numGenerations)
            {
                int numParticles = particlesPerGen[generation];

                // Para cada partícula actual, generamos hijas en su posición
                std::list<Particula*> nuevas;

                for (auto parent : _particles)
                {
                    _generators.front()->setModelP(parent);
                    _generators.front()->setParticulas(numParticles);
                    std::list<Particula*> newParticles;

                    newParticles = _generators.front()->generateP();

                    if (!newParticles.empty())
                    {
                        for (auto p : newParticles)
                        {
                            _registry->add(p, gravityEarth);
                        }

                        // Agregar las nuevas partículas a la lista temporal
                        nuevas.splice(nuevas.end(), newParticles);
                    }
                }

                // Añadir todas las nuevas partículas al sistema
                if (!nuevas.empty())
                {
                    _particles.splice(_particles.end(), nuevas);
                }

                // Preparar siguiente generación
                generation++;
                exploded = false;
                contExplosion = 1.5; // tiempo hasta la próxima explosión

            }
            else
            {
                exploded = true;
                contExplosion = 0.0;
            }
        }
        
    }
    

    // Actualizar las partículas existentes
    for (auto p : _particles)
    {
        p->integrate(t);
    }

    // Eliminar las partículas muertas
    _particles.remove_if([](Particula* p) {
        if (p->getTimeVida() <= 0.0)
        {
            if (p->getRenderItem()) {
                DeregisterRenderItem(p->getRenderItem());
                delete p->getRenderItem();
            }
            delete p;
            return true;
        }
        return false;
        });
}

