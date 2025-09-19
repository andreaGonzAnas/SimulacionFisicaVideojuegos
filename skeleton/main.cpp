#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

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
	gSphere.radius = 2;

	//2. Crear shape
	PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	//3. Crear item (con Transform)
	PxTransform* esferaTr = new PxTransform(PxVec3(0, 0, 0));
	RenderItem* rEsfera = new RenderItem(esferaShape, esferaTr, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	RegisterRenderItem(rEsfera); //y registrar item a renderizar

	//CREAR EJE:
	//1. Eje X
	PxSphereGeometry gSphere1 = PxSphereGeometry();
	gSphere1.radius = 2;
	PxShape* esferaShape1 = CreateShape(gSphere1, gMaterial);
	PxTransform* esferaTr1 = new PxTransform(PxVec3(20, 0, 0));
	RenderItem* rEsfera1 = new RenderItem(esferaShape1, esferaTr1, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RegisterRenderItem(rEsfera1);

	//2. Eje Y
	PxSphereGeometry gSphere2 = PxSphereGeometry();
	gSphere2.radius = 2;
	PxShape* esferaShape2 = CreateShape(gSphere2, gMaterial);
	PxTransform* esferaTr2 = new PxTransform(PxVec3(0, 20, 0));
	RenderItem* rEsfera2 = new RenderItem(esferaShape2, esferaTr2, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RegisterRenderItem(rEsfera2);

	//3. Eje Z
	PxSphereGeometry gSphere3 = PxSphereGeometry();
	gSphere3.radius = 2;
	PxShape* esferaShape3 = CreateShape(gSphere3, gMaterial);
	PxTransform* esferaTr3 = new PxTransform(PxVec3(0, 0, 20));
	RenderItem* rEsfera3 = new RenderItem(esferaShape3, esferaTr3, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RegisterRenderItem(rEsfera3);
	
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
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
	//case 'B': break;
	//case ' ':	break;
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