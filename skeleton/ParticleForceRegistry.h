#pragma once
#include <unordered_map>
#include "ForceGenerator.h"

class ParticleForceRegistry
{
private:
	std::unordered_map<ForceGenerator*, std::vector<Particula*>> forceToParticles;
	std::unordered_map<Particula*, std::vector<ForceGenerator*>> particleToForces;

public:
	ParticleForceRegistry() {};
    
    // Añadir relación fuerza - partícula
    void add(Particula* p, ForceGenerator* fg)
    {
        forceToParticles[fg].push_back(p);
        particleToForces[p].push_back(fg);
    }


    // Eliminar una fuerza completa (de todas las partículas)
    void removeForce(ForceGenerator* fg)
    {
        auto it = forceToParticles.find(fg);
        if (it == forceToParticles.end()) return; //si no hay fuerzas

        for (auto* p : it->second)
        {
            auto& forces = particleToForces[p];
            forces.erase(std::remove(forces.begin(), forces.end(), fg), forces.end());
        }

        forceToParticles.erase(it);
    }

    // Eliminar todas las fuerzas de una partícula
    void removeForcesOfAParticle(Particula* p)
    {
        auto it = particleToForces.find(p);
        if (it == particleToForces.end()) return;

        for (auto* fg : it->second)
        {
            auto& particles = forceToParticles[fg];
            particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
        }

        particleToForces.erase(it);
    }

    // Obtener todas las partículas afectadas por una fuerza
    std::vector<Particula*> getParticlesForForce(ForceGenerator* fg)
    {
        if (forceToParticles.count(fg))
            return forceToParticles[fg];
        return {};
    }

    // Obtener todas las fuerzas que afectan a una partícula
    std::vector<ForceGenerator*> getForcesForParticle(Particula* p)
    {
        if (particleToForces.count(p))
            return particleToForces[p];
        return {};
    }

    // Actualizar todas las fuerzas registradas
    void updateForces(double t) {
        for (auto& pair : forceToParticles) {
            ForceGenerator* fg = pair.first;
            for (auto* p : pair.second) {
                fg->update(p, t);
            }
        }
    }

    void clear()
    {
        forceToParticles.clear();
        particleToForces.clear();
    }
};

