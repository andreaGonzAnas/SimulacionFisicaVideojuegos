#include "Scene0.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"

#include "FireworksParticleSystem.h"
#include "FireParticleSystem.h"
#include "ConfettiParticleSystem.h"
#include "ProyectilSystem.h"

Scene0::Scene0(PxPhysics* physics) : Scene(physics), _firePartSystem(nullptr), _confettiPartSys(nullptr), _fireworkPartSys(nullptr), _proyectilSys(nullptr)
{
}

Scene0::~Scene0()
{
	clear();
}

void Scene0::init()
{
	//Material
	PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// SISTEMA DE PARTICULAS: FUEGO

	// 1. Particula modelo
	double energiaR = 1 / 2 * 0.1 * 50 * 50;

	//energia simulada
	double energiaS = energiaR;
	double masaS = 0.1 * pow((250 / 25), 2); //masa simulada
	Particula* pAux = new Particula(Vector3(35, 40, 35), Vector3(0, 2, 0), 0.98, 0.1);
	pAux->setColor(Vector4(1.0f, 0.3f, 0.05f, 1.0f));
	pAux->setTimeVida(0.3);

	// 2. Sistema de particulas
	_firePartSystem = new FireParticleSystem(pAux, gPhysics);


	// SISTEMA DE PARTICULAS 2: FUEGOS ARTIFICIALES

	// 1. Particula modelo
	energiaR = 1 / 2 * 17 * 250 * 250;

	//energia simulada
	energiaS = energiaR;
	masaS = 17 * pow((250 / 25), 2); //masa simulada
	pAux = new Particula(Vector3(0, 15, 0), Vector3(0, 12, 0), 0.98, masaS);
	pAux->setColor(Vector4(1.0f, 0.5f, 0.0f, 1.0f));

	// 2. Sistema de particulas
	_fireworkPartSys = new FireworksParticleSystem(pAux, gPhysics);

	// SISTEMA DE PARTICULAS 3: CONFETTI

	// 1. Particula modelo
	energiaR = 1 / 2 * 0.1 * 50 * 50;

	//energia simulada
	energiaS = energiaR;
	masaS = 0.1 * pow((250 / 25), 2); //masa simulada
	pAux = new Particula(Vector3(35, 40, 50), Vector3(0, 8, -8), 0.98, 0.1);
	pAux->setColor(Vector4(0.0f, 1.0f, 0.5f, 1.0f));
	pAux->setTimeVida(1.0);

	// 2. Sistema de particulas
	_confettiPartSys = new ConfettiParticleSystem(pAux, gPhysics);

	// SISTEMA PROYECTILES
	_proyectilSys = new ProyectilSystem();
}

void Scene0::update(double t)
{
	_firePartSystem->update(t);
	_fireworkPartSys->update(t);
	_confettiPartSys->update(t);
	_proyectilSys->update(t);
}

void Scene0::clear()
{
	delete _firePartSystem; _firePartSystem = nullptr;
	delete _confettiPartSys; _confettiPartSys = nullptr;
	delete _fireworkPartSys; _fireworkPartSys = nullptr;
	delete _proyectilSys; _proyectilSys = nullptr;
}

bool Scene0::handleKey(unsigned char key, const PxTransform& camera)
{
	PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	switch (key)
	{
		case 'c': // cañon
		{
			//Geometria
			PxSphereGeometry gSphere = PxSphereGeometry();
			gSphere.radius = 0.8;
			physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

			_proyectilSys->createProyectil(Vector4(0.490f, 0.404f, 0.349f, 1.0f), 17, 250, 25, esferaShape);
			break;
		}
		case 't': //tanque
		{
			//Geometria
			PxSphereGeometry gSphere = PxSphereGeometry();
			gSphere.radius = 1.3;
			physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

			_proyectilSys->createProyectil(Vector4(0.392f, 0.514f, 0.459f, 1.0f), 25, 1800, 100, esferaShape);
			break;
		}
		case 'p': //pistola
		{
			//Geometria
			PxSphereGeometry gSphere = PxSphereGeometry();
			gSphere.radius = 0.3;
			physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

			//disparar pistola
			_proyectilSys->createProyectil(Vector4(0.592f, 0.667f, 0.675f, 1.0f), 5, 330, 200, esferaShape);
			break;
		}
		case 'l': //laser
		{
			//Geometria
			PxSphereGeometry gSphere = PxSphereGeometry();
			gSphere.radius = 0.5;
			physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

			//disparar pistola laser
			_proyectilSys->createProyectil(Vector4(1.0f, 0.0f, 0.0f, 1.0f), 2, 300000000, 1100, esferaShape);
			break;
		}
		case 'g':
		{
			_gravityOn = !_gravityOn;
			//desactivar la gravedad de todos los sistemas
			_firePartSystem->setActiveGravity(_gravityOn); //fuego
			_confettiPartSys->setActiveGravity(_gravityOn);
			_fireworkPartSys->setActiveGravity(_gravityOn);
			_proyectilSys->setActiveGravity(_gravityOn);
			break;
		}
		case 'i':
		{
			_windOn = !_windOn;
			//viento
			_firePartSystem->setActiveWind(_windOn);
			_confettiPartSys->setActiveWind(_windOn);
			_fireworkPartSys->setActiveWind(_windOn);
			_proyectilSys->setActiveWind(_windOn);
			break;
		}
		case 'o':
		{
			_windWhirlOn = !_windWhirlOn;
			//torbellino
			_firePartSystem->setActiveWhirlWind(_windWhirlOn);
			_confettiPartSys->setActiveWhirlWind(_windWhirlOn);
			_fireworkPartSys->setActiveWhirlWind(_windWhirlOn);
			_proyectilSys->setActiveWhirlWind(_windWhirlOn);
			break;
		}
		case '3': //confetti
		{
			_confettiPartSys->setActive(!_confettiPartSys->isActive());
			break;
		}
		case '4': //fuego
		{
			_firePartSystem->setActive(!_firePartSystem->isActive());
			break;
		}
		case '5': //fuego
		{
			_fireworkPartSys->setActive(!_fireworkPartSys->isActive());
			break;
		}
		default: return false;
	}
	return true;
}
