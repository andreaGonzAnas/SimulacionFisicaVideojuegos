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


//SceneManager
SceneManager* _sceneManager;
Scene0* _scene0;
Scene1* _scene1;


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
	_scene1 = new Scene1(gPhysics);

	//Setear escena actual
	_sceneManager->setScene(_scene1);

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
			_sceneManager->setScene(new Scene0(gPhysics));

			break;
		}
		case '1':
		{
			//cambiar escena
			_sceneManager->setScene(new Scene1(gPhysics));

			break;
		}

		/*
	case '1':
	{
		//disparar bola de cañon
		createProyectil(Vector4(0.490f, 0.404f, 0.349f, 1.0f), 0.8, 17, 250, 25);

		//disparar tanque
		createProyectil(Vector4(0.392f, 0.514f, 0.459f, 1.0f), 1.3, 25, 1800, 100);
		break;
	}
	case '2':
	{
		_sceneManager->setScene(new Scene0(gPhysics));
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
	case '5':
	{
		_gravityOn = !_gravityOn;
		//desactivar la gravedad de todos los sistemas
		_firePartSystem->setActiveGravity(_gravityOn); //fuego
		_confettiPartSys->setActiveGravity(_gravityOn);
		_fireworkPartSys->setActiveGravity(_gravityOn);
		_proyectilSys->setActiveGravity(_gravityOn);
		break;
	}
	case '6':
	{
		_windOn = !_windOn;
		//viento
		_firePartSystem->setActiveWind(_windOn);
		_confettiPartSys->setActiveWind(_windOn);
		_fireworkPartSys->setActiveWind(_windOn);
		_proyectilSys->setActiveWind(_windOn);
		break;
	}
	case '7':
	{
		_windWhirlOn = !_windWhirlOn;
		//torbellino
		_firePartSystem->setActiveWhirlWind(_windWhirlOn);
		_confettiPartSys->setActiveWhirlWind(_windWhirlOn);
		_fireworkPartSys->setActiveWhirlWind(_windWhirlOn);
		_proyectilSys->setActiveWhirlWind(_windWhirlOn);
		break;
	}
	case '8':
	{
		//quitar generar uniformal
		_confettiPartSys->setActiveUniformal();
		_firePartSystem->setActiveUniformal();

		break;
	}
	case '9':
	{
		//quitar generar uniformal
		_fireworkPartSys->setActiveGaussian();

		break;
	}
	case '0':
	{
		//cambiar escena
		_sceneManager->setScene(new Scene1(gPhysics));

		break;
	}*/
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