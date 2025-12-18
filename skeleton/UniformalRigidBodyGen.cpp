#include "UniformalRigidBodyGen.h"
#include "DynamicObj.h"
#include <algorithm>

UniformalRigidBodyGen::UniformalRigidBodyGen(int nPart, double prob, PxRigidDynamic* p, PxPhysics* gPhysic)
{
	_mt = std::mt19937(std::random_device{}());
	_u = std::uniform_real_distribution<double>(0.0, 1.0);

	// Configuracion general
	setParticulas(nPart);
	setProbGen(prob);

	_modelRigidBody = p;
	gPhysics = gPhysic;
}

UniformalRigidBodyGen::~UniformalRigidBodyGen()
{
}

std::list<DynamicObj*> UniformalRigidBodyGen::generateP()
{
    std::list<DynamicObj*> auxList;

    if (!active) return auxList;

    
    for (int i = 0; i < nRigidBodies; i++)
    {
        // Probabilidad de generar
        if (_u(_mt) < getProbGen())
        {
            // pos
            float offsetX = float((_u(_mt) - 0.5) * 1.5);
            float offsetZ = 0.0f;
            PxVec3 pos = _originPos + PxVec3(offsetX, 1.0f, offsetZ);

            // vel
            float velX = float((_u(_mt) - 0.5) * 12.0f);
            float velY = 25.0f + float(_u(_mt) * 15.0f);
            float velZ = 0.0f;

            PxVec3 linVel = PxVec3(velX, velY, velZ);

            // color
            Vector4 baseC = Vector4(0.23f, 1.0f, 0.84f, 1.0f);
            float randomVal = static_cast<float>(_u(_mt));    // Aleatorio entre 0.0 y 1.0

            Vector4 newC = Vector4(
                std::clamp(baseC.x, 0.1f, 0.3f),                
                1.0f,                                               
                std::clamp(baseC.z - (randomVal * 0.6f), 0.0f, 0.5f),
                1.0f
            );

            // Variación de material
            float staticFriction = 0.0f;    // 0 a 0.3
            float dynamicFriction = 0.0f;   // 0 a 0.3
            float restitution = 0.9f + _u(_mt) * 0.1f;       // 0.6 a 1.0

            PxMaterial* material = gPhysics->createMaterial(staticFriction, dynamicFriction, restitution);

            // escala
            float scale = 0.1f + static_cast<float>(_u(_mt)) * 0.3f;

            // geometria
            PxBoxGeometry confettiGeom = PxBoxGeometry(scale, scale, scale);
            PxShape* shape = gPhysics->createShape(confettiGeom, *material);

            // densidad
            float density = 0.1f * (scale * 10.0f);

            // DynamicObj
            DynamicObj* obj = new DynamicObj(
                linVel,
                PxVec3(0.0f),
                shape,
                pos,
                density,
                5.0, // tiempo de vida
                gPhysics,
                newC
            );
            auxList.push_back(obj);
        }
    }

    return auxList;
}

