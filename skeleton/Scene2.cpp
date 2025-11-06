#include "Scene2.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <iostream>

#include "FireParticleSystem.h"
#include "ProyectilSystem.h"
#include "FireworksParticleSystem.h"
#include "ConfettiParticleSystem.h"

Scene2::Scene2(PxPhysics* physics): Scene(physics), _hasPassedFire(false)
{

}

Scene2::~Scene2()
{
	clear();
}

void Scene2::init()
{
	//---- CIRCULO FUEGOS -----
    int numFuegos = 16;   // Número de fuegos en el círculo
    float radio = 8.0f; // Radio del círculo
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
        _firePartSystem->setActiveWind(true);

        _firesInScene.push_back(_firePartSystem);
    }

    // ---- POSICION CAMARA ----
    Camera* cam = GetCamera();
    _initPosCamera = cam->getEye();
    _initDirCamera = cam->getDir();
    cam->setTransform(PxVec3(centro.x, centro.y - 10.0, centro.z - 45.0));
    cam->setDir(PxVec3(0.0, 0.0, 1));


    // ---- PROYECTIL ----
    _proyectilSys = new ProyectilSystem();

    // ---- CONFETTI ----

    // confetti en cada esquina de la camara
    PxVec3 camPos = cam->getTransform().p;
    PxVec3 camDir = cam->getDir();
    camDir.normalize();

    PxVec3 worldUp(0, 1, 0);
    PxVec3 right = camDir.cross(worldUp).getNormalized();
    PxVec3 up = right.cross(camDir).getNormalized();

    float distancia = 33.0f;
    float offset = 15.0f;

    std::vector<PxVec3> posiciones = {
        camPos + camDir * distancia + up * offset - right * offset,  // arriba izquierda
        camPos + camDir * distancia + up * offset + right * offset,  // arriba derecha
        camPos + camDir * distancia - up * offset - right * offset,  // abajo izquierda
        camPos + camDir * distancia - up * offset + right * offset   // abajo derecha
    };

    int nConfettis = 4;
    for (int i = 0; i < nConfettis; ++i)
    {
        PxVec3 pos = posiciones[i];

        // Energías (si quieres mantener tu cálculo original)
        double energiaR = 0.5 * 0.1 * 50 * 50;
        double energiaS = energiaR;
        double masaS = 0.1 * pow((250 / 25), 2);

        // Crear partícula base
        Particula* pAux = new Particula(Vector3(pos.x, pos.y - 5.0, pos.z),
            Vector3(0, 8, -8), 0.98, 0.1);
        pAux->setColor(Vector4(0.0f, 1.0f, 0.5f, 1.0f));
        pAux->setTimeVida(1.0);

        // Crear sistema de confetti
        ConfettiParticleSystem* _confettiPartSys = new ConfettiParticleSystem(pAux, gPhysics);
        _confettis.push_back(_confettiPartSys);
    }
    
    // desactivar confettis al principio
    for (auto a : _confettis)
    {
        a->setActive(false);
    }


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

    if (!_proyectilSys->getParticleList().empty() && !_hasPassedFire)
    {
        // comprobar que si ha pasado el circulo de fuego
        for (auto p : _proyectilSys->getParticleList())
        {
            Vector3 pos = p->getPos();

            // Datos del círculo de fuego
            PxVec3 centro(35, 40, 35);
            float radio = 8.0f;
            float alturaMin = 37.0f; // margen inferior
            float alturaMax = 43.0f; // margen superior

            // Comprobar si el proyectil pasa por el interior del círculo (en planta XZ)
            float dx = pos.x - centro.x;
            float dz = pos.z - centro.z;
            float distXZ = sqrt(dx * dx + dz * dz);

            // Dentro del cilindro si está dentro del radio y altura
            if (distXZ <= radio && pos.y >= alturaMin && pos.y <= alturaMax)
            {
                std::cout << "El proyectil ha pasado por el círculo de fuego!" << '\n';
                
                // Activar confetti y fuegos artificiales
                startCelebration();

                _hasPassedFire = true;
            }
        }
    }
    else
    {
        //Actualizar confetti y fuegos artificiales
        for (auto a : _confettis)
        {
            a->update(t);
        }
    }
    
}

void Scene2::clear()
{
    //Eliminar todos los objetos creados

    //Volver camara a la posicion inicial
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

            // Masa y velocidad real propias de un human cannonball
            // masa = 70 kg
            // vel = 33 m/s

            _proyectilSys->createProyectil(Vector4(0.490f, 0.404f, 0.349f, 1.0f), 70, 33, 40, esferaShape);

            break;
        }
        default: return false;
    }
	return true;
}

void Scene2::startCelebration()
{
    std::cout << "Activar celebracion" << '\n';

    //Activar confetti
    for (auto a : _confettis)
    {
        a->setActive(true);
    }

    //Activar fuegos artificiales

}
