#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"
#include "Particula.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath> 

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

Particula* p;

std::vector<Particula*> _bullets;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);


	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	//ESFERA:
	
	//1. Crear geometria
	PxSphereGeometry gSphere = PxSphereGeometry();
	gSphere.radius = 1.5;

	//2. Crear shape
	PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	//3. Crear item (con Transform)
	PxTransform* esferaTr = new PxTransform(PxVec3(0, 0, 0));
	RenderItem* rEsfera = new RenderItem(esferaShape, esferaTr, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	RegisterRenderItem(rEsfera); //y registrar item a renderizar

	//CREAR EJE:
	//Vectores:
	Vector3D ejeX = Vector3D(10, 0, 0);
	Vector3D ejeY = Vector3D(0, 10, 0);
	Vector3D ejeZ = Vector3D(0, 0, 10);

	//1. Eje X
	PxTransform* esferaTr1 = new PxTransform(PxVec3(ejeX.getX(), ejeX.getY(), ejeX.getZ()));
	RenderItem* rEsfera1 = new RenderItem(esferaShape, esferaTr1, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RegisterRenderItem(rEsfera1);

	//2. Eje Y
	PxTransform* esferaTr2 = new PxTransform(PxVec3(ejeY.getX(), ejeY.getY(), ejeY.getZ()));
	RenderItem* rEsfera2 = new RenderItem(esferaShape, esferaTr2, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RegisterRenderItem(rEsfera2);

	//3. Eje Z
	PxTransform* esferaTr3 = new PxTransform(PxVec3(ejeZ.getX(), ejeZ.getY(), ejeZ.getZ()));
	RenderItem* rEsfera3 = new RenderItem(esferaShape, esferaTr3, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RegisterRenderItem(rEsfera3);

	/*
	//PARTICULA
	p = new Particula(Vector3(0, 50, 0), Vector3(0, 0.5, 0), Vector3(0, -0.5, 0), 0.98);
	RenderItem* renderItem = new RenderItem(esferaShape, p->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	p->setRenderItem(renderItem);*/

	//PROYECTIL (bala CA�ON)
	//p->setMasa(40.0);
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	if (p != nullptr)
	{
		/*
		p->integrate(t);
		if (p->getTimeVida() <= 0.0)
		{
			delete p;
			p = nullptr;
		}*/
	}

	//balas
	if (!_bullets.empty())
	{
		for (auto b : _bullets)
			b->integrate(t);
	}

	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	//slow down the simulation, necessary for Verlet integration
	//TODO: review the physics timestep and the integrator stability
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	}

void createProyectil(Vector4 color, double size, double masaR, double velR, double velS)
{
	//Geometria
	PxSphereGeometry gSphere = PxSphereGeometry();
	gSphere.radius = size;
	PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	//PARA PROYECTIL

	//calcular la energia real
	//double masaR = 17; //kg
	//double velR = 250; // m/s
	

	double energiaR = 1 / 2 * masaR * velR * velR;
	
	//energia simulada
	double energiaS = energiaR;
	//double velS = 25; // m/s velocidad simulada (la que quiero utilizar)

	double masaS = masaR * pow((velR / velS), 2); //masa simulada

	Vector3 pos = GetCamera()->getTransform().p; // pos inicial ca�on = pos de la camara
	Vector3 vel = GetCamera()->getDir() * velS; //velocidad inicial: la direccion es la de la camara * velocidad simulada
	
	Vector3 aceleracion(0, -9.8, 0); //la aceleracion del proyectil es solo la gravedad

	Particula* pAux = new Particula(pos, vel, aceleracion, 0.98, masaS);
	RenderItem* renderItem = new RenderItem(esferaShape, pAux->getTr(), color);
	pAux->setRenderItem(renderItem);

	_bullets.push_back(pAux);
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case '1':
	{
		//disparar bola de ca�on
		createProyectil(Vector4(0.490f, 0.404f, 0.349f, 1.0f), 0.8, 17, 250, 25);
		break;
	}
	case '2':
	{
		//disparar tanque
		createProyectil(Vector4(0.392f, 0.514f, 0.459f, 1.0f), 1.3, 25, 1800, 100);
		break;
	}
	case '3':
	{
		//disparar pistola
		createProyectil(Vector4(0.592f, 0.667f, 0.675f, 1.0f), 0.3, 5, 330, 200);
		break;
	}
	case '4':
	{
		//disparar pistola laser
		createProyectil(Vector4(1.0f, 0.0f, 0.0f, 1.0f), 0.5, 2, 300000000, 1100);
		break;
	}
	case ' ':
	{
		break;
	}
	
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}