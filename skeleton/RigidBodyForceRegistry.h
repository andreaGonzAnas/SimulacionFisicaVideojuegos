#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "PxPhysicsAPI.h"
#include "ForceGenerator.h"


class RigidBodyForceRegistry
{
private:
    std::unordered_map<ForceGenerator*, std::vector<physx::PxRigidDynamic*>> forceToBodies;
    std::unordered_map<physx::PxRigidDynamic*, std::vector<ForceGenerator*>> bodyToForces;

public:
    RigidBodyForceRegistry() {}

    // Añadir relación fuerza - rigid body
    void add(physx::PxRigidDynamic* body, ForceGenerator* fg)
    {
        forceToBodies[fg].push_back(body);
        bodyToForces[body].push_back(fg);
    }

    // Eliminar una fuerza completa
    void removeForce(ForceGenerator* fg)
    {
        auto it = forceToBodies.find(fg);
        if (it == forceToBodies.end()) return;

        for (auto* body : it->second)
        {
            auto& forces = bodyToForces[body];
            forces.erase(std::remove(forces.begin(), forces.end(), fg), forces.end());
        }

        forceToBodies.erase(it);
    }

    // Eliminar todas las fuerzas de un rigid body
    void removeForcesOfABody(physx::PxRigidDynamic* body)
    {
        auto it = bodyToForces.find(body);
        if (it == bodyToForces.end()) return;

        for (auto* fg : it->second)
        {
            auto& bodies = forceToBodies[fg];
            bodies.erase(std::remove(bodies.begin(), bodies.end(), body), bodies.end());
        }

        bodyToForces.erase(it);
    }

    // Actualizar todas las fuerzas
    void updateForces(double t)
    {
        for (auto& pair : forceToBodies)
        {
            ForceGenerator* fg = pair.first;
            if (fg->isActive())
            {
                for (auto* body : pair.second)
                {
                    fg->updateRigidBody(body, t); // Necesitas implementar updateRigidBody en tus ForceGenerators
                }
            }
        }
    }

    void clear()
    {
        forceToBodies.clear();
        bodyToForces.clear();
    }

    void remove(physx::PxRigidDynamic* body)
    {
        auto it = bodyToForces.find(body);
        if (it == bodyToForces.end()) return;

        for (auto* fg : it->second)
        {
            auto& bodies = forceToBodies[fg];
            bodies.erase(std::remove(bodies.begin(), bodies.end(), body), bodies.end());
        }

        bodyToForces.erase(it);
    }
};

