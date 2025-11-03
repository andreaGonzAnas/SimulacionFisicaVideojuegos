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
    active = false;

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
}

FireworksParticleSystem::~FireworksParticleSystem()
{
}

/*
void FireParticleSystem::update(double t)
{
    // Actualizar fuerzas
    _registry->updateForces(t);

    // Reducir contador de duracion de la explosión
    /*
    if (active)
    {
        contExplosion -= t;

        if (contExplosion <= 0.0)
        {
            contExplosion = 0.0;
            active = false;
            std::cout << "La explosión terminó — ya no se generan nuevas partículas\n";
        }
    }

    if (!exploded)
    {
        contExplosion -= t;

        if (contExplosion <= 0.0)
        {
            contExplosion = 0.0;
            exploded = true;
            std::cout << "La explosion debe comenzar";
        }
    }
    else
    {
        //generar x particulas segun el nivel de generacion
        if (generation == 1)
        {
            //generar 10
            _generators.front()->setParticulas(10);
            auto newParticles = _generators.front()->generateP();
            if (!newParticles.empty()) {
                for (auto p : newParticles) {
                    _registry->add(p, gravityEarth);
                }
                _particles.splice(_particles.end(), newParticles);
            }
            generation++;
        }
        else if (generation == 2)
        {
            //generar 10
            _generators.front()->setParticulas(10);
            auto newParticles = _generators.front()->generateP();
            if (!newParticles.empty()) {
                for (auto p : newParticles) {
                    _registry->add(p, gravityEarth);
                }
                _particles.splice(_particles.end(), newParticles);
            }
            generation++;
        }
    }

    
    /*
    // Generar nuevas particulas SOLO si sigue activa
    if (active && !_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty()) {
            for (auto p : newParticles) {
                _registry->add(p, gravityEarth);
            }
            _particles.splice(_particles.end(), newParticles);
        }
    }

    // Actualizar particulas existentes
    for (auto p : _particles) {
        p->integrate(t);
    }

    // Eliminar particulas muertas
    _particles.remove_if([](Particula* p) {
        if (p->getTimeVida() <= 0.0) {
            if (p->getRenderItem()) {
                DeregisterRenderItem(p->getRenderItem());
                delete p->getRenderItem();
            }
            delete p;
            return true;
        }
        return false;
        });
}*/

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
            std::cout << "La explosión debe comenzar (Generación " << generation << ")\n";
            
        }
    }
    else
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
                auto newParticles = _generators.front()->generateP();

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
                std::cout << "Generación " << generation
                    << " creada con " << numParticles << " partículas por partícula base\n";
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
            std::cout << "Fin de las generaciones\n";
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

