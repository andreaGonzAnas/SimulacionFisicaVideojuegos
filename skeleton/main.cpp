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
#include "InitialMenuScene.h"
#include "LevelMenuScene.h"


#include <iostream>
#include <chrono>
#include <thread>
#include <cmath> 

// Textos minijuego personas balas
std::string display_title = "¡DISPARA AL HOMBRE BALA!";
std::string display_rossa = "PULSA R Y DISPARA A ROSSA (PRIMERA PERSONA BALA DE LA HISTORIA)";
std::string display_left = "PULSA C Y DISPARA A CALEB";
std::string display_right = "PULSA T Y DISPARA A TIM";
std::string display_score = "PUNTUACION: 0";

// Textos minijuego trapecios
std::string display_titleT = "¡DALE UNA OPORTUNIDAD A LOS TRAPECIOS!";
std::string display_instr1 = "SALTA POR LOS TRAPECIOS, RECOGE LA PARTICULA COLGANDO";
std::string display_instr2 = "Y LLEGA A LA PLATAFORMA FINAL";
std::string display_instr3 = "PULSA T PARA INICIAR LA PARTIDA";
std::string display_instr4 = "PULSA ESPACIO PARA SALTAR";

// Victoria
std::string display_win = "¡VICTORIA!";
std::string display_reiniciarWin = "Volviendo al menu en 3 segundos...";

// GameOver
std::string display_lose = "¡TE CAISTE!";
std::string display_reiniciarLose = "Reiniciando en 3 segundos...";

// Initial Menu
std::string initial_title = "SIMULADOR DE CIRCO";
std::string initial_start = "START";
std::string initial_exit = "EXIT";

// Level Menu
std::string level_title = "ELIGE ACTO";
std::string level_bala = "HUMANO BALA";
std::string level_trapecios = "TRAPECIOS";
std::string level_back = "VOLVER AL INICIO";


bool _personas_balas = false;
bool _victory = false;
bool _lose = false;
bool _trapeciosText = false;
bool _initialMenu = false;
bool _levelMenu = false;

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
InitialMenuScene* _initialMenuScene;
LevelMenuScene* _levelMenuScene;


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

	_initialMenuScene = new InitialMenuScene(gPhysics, gScene);

	_levelMenuScene = new LevelMenuScene(gPhysics, gScene);

	//Setear escena actual
	_sceneManager->setScene(_initialMenuScene);
	
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	//Update de la escena
	_sceneManager->update(t);

	if (_sceneManager->getCurrentScene()->getType() == 0)
	{
		// Convertimos el puntero de Scene* a InitialMenuScene*
		InitialMenuScene* menu = static_cast<InitialMenuScene*>(_sceneManager->getCurrentScene());

		// Ahora ya puedes acceder al método específico
		auto result = menu->getButtonResult();

		if (result) //cambiar a changeLevel
		{
			_sceneManager->setScene(new LevelMenuScene(gPhysics, gScene));
		}
	}

	if (_sceneManager->getCurrentScene()->getType() == 2)
	{
		// Convertimos el puntero de Scene* a InitialMenuScene*
		LevelMenuScene* menu = static_cast<LevelMenuScene*>(_sceneManager->getCurrentScene());

		// Ahora ya puedes acceder al método específico
		auto result = menu->getButtonResult();

		if (result == 1) //cambiar a balas
		{
			_scene2->set_gScene(gScene);
			_sceneManager->setScene(new Scene2(gPhysics));
		}
		else if (result == 2)
		{
			_sceneManager->setScene(new SceneTrapecios(gPhysics, gScene));
		}
		else if (result == 0)
		{
			_sceneManager->setScene(new InitialMenuScene(gPhysics, gScene));
		}
	}

	// juego
	if (_sceneManager->getCurrentScene()->getType() == 1)
	{
		// Convertimos el puntero de Scene* a InitialMenuScene*
		SceneTrapecios* trapecios = static_cast<SceneTrapecios*>(_sceneManager->getCurrentScene());
		Scene2* balas = static_cast<Scene2*>(_sceneManager->getCurrentScene());

		// Ahora ya puedes acceder al método específico
		auto resultTrapecios = trapecios->getChangeScene();
		auto resultBalas = balas->getChangeScene();

		if (resultTrapecios || resultBalas) //cambiar a level
		{
			_sceneManager->setScene(new LevelMenuScene(gPhysics, gScene));
		}
	}

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