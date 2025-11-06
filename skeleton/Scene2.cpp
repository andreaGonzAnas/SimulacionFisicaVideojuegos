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

        double energiaR = 0.5 * 0.1 * 50 * 50;
        double energiaS = energiaR;
        double masaS = 0.1 * pow((250 / 25), 2);

        // particula base
        Particula* pAux = new Particula(Vector3(pos.x, pos.y - 5.0, pos.z),
            Vector3(0, 8, -8), 0.98, 0.1);
        pAux->setColor(Vector4(0.0f, 1.0f, 0.5f, 1.0f));
        pAux->setTimeVida(1.0);

        // confetti
        ConfettiParticleSystem* _confettiPartSys = new ConfettiParticleSystem(pAux, gPhysics);
        _confettis.push_back(_confettiPartSys);
    }
    
    // desactivar confettis al principio
    for (auto a : _confettis)
    {
        a->setActive(false);
    }
    
    _firework = nullptr;
    

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

        if (_firework != nullptr)
        {
            if (_firework->getEnd())
            {
                createNewFirework();
            }
            else
            {
                _firework->update(t);
            }
        }
        else if(_hasPassedFire)
        {
            createNewFirework();
        }
    }

}

void Scene2::clear()
{
    //Eliminar todos los objetos creados
    for (auto*& a : _firesInScene)
    {
        delete a;
        a = nullptr;
    }
    _firesInScene.clear();

    for (auto*& a : _confettis)
    {
        delete a;
        a = nullptr;
    }
    _confettis.clear();

    delete _firework; _firework = nullptr;
    delete _proyectilSys; _proyectilSys = nullptr;

    //Volver camara a la posicion inicial
    Camera* cam = GetCamera();
    cam->setTransform(_initPosCamera);
    cam->setDir(_initDirCamera);
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
        case 'g':
        {
            _gravityOn = !_gravityOn;
            
            //desactivar la gravedad de todos los sistemas
            
            for (auto a : _firesInScene)
            {
                a->setActiveGravity(_gravityOn);
            }
            
            for (auto a : _confettis)
            {
                a->setActiveGravity(_gravityOn);
            }

            if(_firework != nullptr) _firework->setActiveGravity(_gravityOn);
            _proyectilSys->setActiveGravity(_gravityOn);
            break;
        }
        case 'i':
        {
            _windOn = !_windOn;
            //viento
            
            for (auto a : _firesInScene)
            {
                a->setActiveWind(_windOn);
            }

            for (auto a : _confettis)
            {
                a->setActiveWind(_windOn);
            }

            if (_firework != nullptr) _firework->setActiveWind(_windOn);
            _proyectilSys->setActiveWind(_windOn);
            break;
        }
        case 'o':
        {
            _windWhirlOn = !_windWhirlOn;
            //torbellino
            for (auto a : _firesInScene)
            {
                a->setActiveWhirlWind(_windWhirlOn);
            }

            for (auto a : _confettis)
            {
                a->setActiveWhirlWind(_windWhirlOn);
            }
            if (_firework != nullptr) _firework->setActiveWhirlWind(_windWhirlOn);
            _proyectilSys->setActiveWhirlWind(_windWhirlOn);
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

    //Desactivar viento en fuego
    for (auto a : _firesInScene)
    {
        a->setActiveWind(false);
        a->setActiveWhirlWind(true);
    }
}

void Scene2::createNewFirework()
{
    Camera* cam = GetCamera();
    PxVec3 camPos = cam->getTransform().p;

    double energiaR = 1 / 2 * 17 * 250 * 250;
    double energiaS = energiaR;
    double masaS = 17 * pow((250 / 25), 2); //masa simulada

    Particula* pAux = new Particula(Vector3(camPos.x, camPos.y - 20.0f, camPos.z + 100.0f), Vector3(0, 20, 0), 0.98, masaS);
    pAux->setColor(Vector4(0.0f, 1.0f, 0.5f, 1.0f));

    // 2. Sistema de particulas
    FireworksParticleSystem* _fireworkPartSys = new FireworksParticleSystem(pAux, gPhysics);
    if(_firework != nullptr) delete _firework;
    _firework = _fireworkPartSys;
    
}
