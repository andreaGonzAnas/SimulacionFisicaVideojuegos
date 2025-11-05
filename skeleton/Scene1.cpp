#include "Scene1.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"

#include "FireworksParticleSystem.h"
#include "FireParticleSystem.h"
#include "ConfettiParticleSystem.h"
#include "ProyectilSystem.h"

Scene1::Scene1(PxPhysics* physics) : Scene(physics), _firePartSystem(nullptr), _confettiPartSys(nullptr), _fireworkPartSys(nullptr), _proyectilSys(nullptr)
{
}

Scene1::~Scene1()
{
	clear();
}

void Scene1::init()
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

void Scene1::update(double t)
{
	_firePartSystem->update(t);
	_fireworkPartSys->update(t);
	_confettiPartSys->update(t);
	_proyectilSys->update(t);
}

void Scene1::clear()
{
	delete _firePartSystem; _firePartSystem = nullptr;
	delete _confettiPartSys; _confettiPartSys = nullptr;
	delete _fireworkPartSys; _fireworkPartSys = nullptr;
	delete _proyectilSys; _proyectilSys = nullptr;
}
