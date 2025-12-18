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

    //desColor = Vector4(0.8f, 0.0f, 0.9f, 1.0f); // base morado brillante



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
            // 1. Posición: Justo en el origen del sistema (donde esté la plataforma)
            float offsetX = float((_u(_mt) - 0.5) * 1.5); // Dispersión pequeña en la base
            float offsetZ = 0.0f; // Obligamos a que se quede en el plano del juego
            PxVec3 pos = _originPos + PxVec3(offsetX, 1.0f, offsetZ);

            // 2. Velocidad de FUENTE:
            // velX: hace que se abran hacia los lados (abanico)
            float velX = float((_u(_mt) - 0.5) * 12.0f);

            // velY: IMPULSO HACIA ARRIBA (Debe ser positivo y alto)
            float velY = 25.0f + float(_u(_mt) * 15.0f); // Sale disparado entre 25 y 40 hacia arriba

            // velZ: Siempre 0 para mantener el plano XY
            float velZ = 0.0f;

            PxVec3 linVel = PxVec3(velX, velY, velZ);

            // Color con ligera variación
            Vector4 baseC = Vector4(0.8f, 0.0f, 0.8f, 1.0f);
            Vector4 newC = Vector4(
                std::clamp<float>(baseC.x + static_cast<float>(_u(_mt)) * 0.4f, 0.0f, 1.0f),
                std::clamp<float>(baseC.y + static_cast<float>(_u(_mt)) * 0.5f, 0.0f, 1.0f),
                std::clamp<float>(baseC.z + static_cast<float>(_u(_mt)) * 0.2f, 0.7f, 1.0f),
                baseC.w
            );

            // Variación de material
            float staticFriction = 0.0f;    // 0 a 0.3
            float dynamicFriction = 0.0f;   // 0 a 0.3
            float restitution = 0.9f + _u(_mt) * 0.1f;       // 0.6 a 1.0

            PxMaterial* material = gPhysics->createMaterial(staticFriction, dynamicFriction, restitution);

            // 1. Definir un factor de escala aleatorio (ej. entre 0.1 y 0.4)
            float scale = 0.1f + static_cast<float>(_u(_mt)) * 0.3f;

            // 2. Crear la geometría con ese tamaño aleatorio
            // PxBoxGeometry recibe los "half-extents" (la mitad del tamaño total)
            PxBoxGeometry confettiGeom = PxBoxGeometry(scale, scale, scale);
            PxShape* shape = gPhysics->createShape(confettiGeom, *material);

            // 3. (Opcional) Ajustar la densidad según el tamaño
            // Si quieres que las piezas grandes pesen más:
            float density = 0.1f * (scale * 10.0f);

            // 4. Crear el DynamicObj con la forma escalada
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

