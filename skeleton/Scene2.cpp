#include "Scene2.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"

#include "FireParticleSystem.h"
#include "ProyectilSystem.h"

Scene2::Scene2(PxPhysics* physics): Scene(physics)
{

}

Scene2::~Scene2()
{
	clear();
}

void Scene2::init()
{
	//---- CIRCULO FUEGOS -----
    int numFuegos = 8;   // Número de fuegos en el círculo
    float radio = 10.0f; // Radio del círculo
    PxVec3 centro(35, 40, 35); // Centro del círculo

    for (int i = 0; i < numFuegos; ++i)
    {
        // Ángulo actual en radianes
        float ang = (2 * PxPi * i) / numFuegos;

        // Posición en círculo (plano XY)
        float x = centro.x + radio * cos(ang);
        float y = centro.y + radio * sin(ang);
        float z = centro.z; // z fijo

        // Crear partícula modelo
        Particula* pAux = new Particula(Vector3(x, y, z), Vector3(0, 2, 0), 0.98, 0.1);
        pAux->setColor(Vector4(1.0f, 0.3f, 0.05f, 1.0f));
        pAux->setTimeVida(0.3);

        // Crear sistema de fuego
        FireParticleSystem* _firePartSystem = new FireParticleSystem(pAux, gPhysics);
        _firePartSystem->setActiveWhirlWind(false);

        _firesInScene.push_back(_firePartSystem);
    }

    // ---- POSICION CAMARA ----

    // ---- PROYECTIL ----
    _proyectilSys = new ProyectilSystem();

}

void Scene2::update(double t)
{
	// ---- CIRCULO FUEGOS -----
	for (auto a : _firesInScene)
	{
		a->update(t);
	}

    // ---- PROYECTIL ----
    _proyectilSys->update(t);
}

void Scene2::clear()
{

}

bool Scene2::handleKey(unsigned char key, const PxTransform& camera)
{
    switch (key)
    {
        case 'c': // cañon
        {
            //Geometria
            PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
            PxSphereGeometry gSphere = PxSphereGeometry();
            gSphere.radius = 0.8;
            physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

            _proyectilSys->createProyectil(Vector4(0.490f, 0.404f, 0.349f, 1.0f), 17, 250, 25, esferaShape);
            break;
        }
        default: return false;
    }
	return true;
}
