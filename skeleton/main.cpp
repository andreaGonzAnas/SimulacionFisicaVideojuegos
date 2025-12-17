#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"
#include "Particula.h"
#include "ParticleSystem.h"
#include "FireworksParticleSystem.h"
#include "FireParticleSystem.h"
#include "ConfettiParticleSystem.h"
#include "ProyectilSystem.h"
#include "Proyectil.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ParticleForceRegistry.h"
#include "SceneManager.h"
#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "SceneTrapecios.h"
#include "MalabaresScene.h"


#include <iostream>
#include <chrono>
#include <thread>
#include <cmath> 

std::string display_title = "¡DISPARA AL HOMBRE BALA!";
std::string display_left = "PULSA C Y DISPARA A CALEB";
std::string display_right = "PULSA T Y DISPARA A TIM";
std::string display_score = "PUNTUACION: 0";
bool isGame = false;

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


//SceneManager
SceneManager* _sceneManager;
Scene* _scene0;
Scene* _scene1;
Scene* _scene2;
Scene* _scene3;
SceneTrapecios* _sceneTrapecios;
MalabaresScene* _sceneMalabares;


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

	//-------------------------------------------------------------

	//Inicializar SceneManager +++++++++++++++++++++++++++++++++++++
	_sceneManager = new SceneManager();

	//Inicializar escena
	_scene0 = new Scene0(gPhysics);
	_scene0->set_gScene(gScene);

	_scene1 = new Scene1(gPhysics);
	_scene1->set_gScene(gScene);

	_scene2 = new Scene2(gPhysics);
	_scene2->set_gScene(gScene);

	_scene3 = new Scene3(gPhysics, gScene);

	_sceneTrapecios = new SceneTrapecios(gPhysics, gScene);

	_sceneMalabares = new MalabaresScene(gPhysics, gScene);

	//Setear escena actual
	_sceneManager->setScene(_sceneMalabares);
	

}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	//Update de la escena
	_sceneManager->update(t);

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



// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
		case '0':
		{
			//cambiar escena
			_scene0->set_gScene(gScene);
			_sceneManager->setScene(new Scene0(gPhysics));
			break;
		}
		case '1':
		{
			//cambiar escena
			_scene1->set_gScene(gScene);
			_sceneManager->setScene(new Scene1(gPhysics));
			break;
		}
		case '2':
		{
			//cambiar escena
			_scene2->set_gScene(gScene);
			_sceneManager->setScene(new Scene2(gPhysics));
			break;
		}
		case '3':
		{
			//cambiar escena
			_sceneManager->setScene(new Scene3(gPhysics, gScene));
			break;
		}
		case '4':
		{
			//cambiar escena
			_sceneManager->setScene(new SceneTrapecios(gPhysics, gScene));
			break;
		}
		case '5':
		{
			//cambiar escena
			_sceneMalabares = new MalabaresScene(gPhysics, gScene);
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