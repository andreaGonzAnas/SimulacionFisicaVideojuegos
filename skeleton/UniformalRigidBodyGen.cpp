#include "UniformalRigidBodyGen.h"
#include "DynamicObj.h"
#include <algorithm>

UniformalRigidBodyGen::UniformalRigidBodyGen(int nPart, double prob, PxRigidDynamic* p, PxPhysics* gPhysic)
{
	_mt = std::mt19937(std::random_device{}());
	_u = std::uniform_real_distribution<double>(-1.0, 1.0);

	// Configuracion general
	setParticulas(nPart);
	setProbGen(prob);

	_modelRigidBody = p;
	gPhysics = gPhysic;

    desColor = Vector4(0.8f, 0.0f, 0.9f, 1.0f); // base morado brillante



}

UniformalRigidBodyGen::~UniformalRigidBodyGen()
{
}

std::list<DynamicObj*> UniformalRigidBodyGen::generateP()
{
    std::list<DynamicObj*> auxList;

    if (!active) return auxList;

    // Crear forma básica
    PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));

    for (int i = 0; i < nRigidBodies; i++)
    {
        // Probabilidad de generar
        if (_u(_mt) < getProbGen())
        {
            // Posición inicial: base de la fuente con desviación aleatoria en X/Z
            float offsetX = float((_u(_mt) - 0.5) * 10.0); // ±5 unidades
            float offsetZ = float((_u(_mt) - 0.5) * 10.0); // ±5 unidades
            PxVec3 pos = PxVec3(50.0f + offsetX, 200.0f, -80.0f + offsetZ);

            // Velocidad inicial: hacia abajo (Y negativa), con ligera dispersión en X/Z
            float velX = float((_u(_mt) - 0.5) * 2.0);   // ±1 unidades/s
            float velY = -10.0f;                         // fuerza hacia abajo
            float velZ = float((_u(_mt) - 0.5) * 2.0);   // ±1 unidades/s
            PxVec3 linVel = PxVec3(velX, velY, velZ);

            // Color con ligera variación
            Vector4 baseC = desColor;
            Vector4 newC = Vector4(
                std::clamp<float>(baseC.x + static_cast<float>(_u(_mt)) * desColor.x, 0.0f, 1.0f),
                std::clamp<float>(baseC.y + static_cast<float>(_u(_mt)) * desColor.y, 0.0f, 1.0f),
                std::clamp<float>(baseC.z + static_cast<float>(_u(_mt)) * desColor.z, 0.0f, 1.0f),
                baseC.w
            );


            // Crear DynamicObj y asignar RenderItem con color
            DynamicObj* obj = new DynamicObj(
                linVel,                         // velocidad inicial
                PxVec3(0.0f),                   // sin rotación inicial
                shape_ad,                        // forma
                pos,                             // posición inicial
                0.15f,                           // densidad
                5.0,                             // tiempo de vida
                gPhysics,
                newC
            );
            auxList.push_back(obj);
        }
    }

    return auxList;
}

