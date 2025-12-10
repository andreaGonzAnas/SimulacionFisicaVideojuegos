#include "Scene3.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "ExplosionRigidBodySystem.h"

#include <PxPhysicsAPI.h>

Scene3::Scene3(PxPhysics* physics): Scene(physics)
{
}

Scene3::~Scene3()
{
    clear();
}

void Scene3::init()
{
	//Material
	PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	//Crear eje de coordenadas
	//ESFERA:

	//1. Crear geometria
	PxSphereGeometry gSphere = PxSphereGeometry();
	gSphere.radius = 1.5;

	//2. Crear shape
	physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	//3. Crear item (con Transform)
	PxTransform* esferaTr = new PxTransform(PxVec3(0, 0, 0));
	RenderItem* rEsfera = new RenderItem(esferaShape, esferaTr, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	RegisterRenderItem(rEsfera); //y registrar item a renderizar
	_parts.push_back(rEsfera);

	//CREAR EJE:
	//Vectores:
	Vector3D ejeX = Vector3D(10, 0, 0);
	Vector3D ejeY = Vector3D(0, 10, 0);
	Vector3D ejeZ = Vector3D(0, 0, 10);

	//1. Eje X
	PxTransform* esferaTr1 = new PxTransform(PxVec3(ejeX.getX(), ejeX.getY(), ejeX.getZ()));
	RenderItem* rEsfera1 = new RenderItem(esferaShape, esferaTr1, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RegisterRenderItem(rEsfera1);
	_parts.push_back(rEsfera1);

	//2. Eje Y
	PxTransform* esferaTr2 = new PxTransform(PxVec3(ejeY.getX(), ejeY.getY(), ejeY.getZ()));
	RenderItem* rEsfera2 = new RenderItem(esferaShape, esferaTr2, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RegisterRenderItem(rEsfera2);
	_parts.push_back(rEsfera2);

	//3. Eje Z
	PxTransform* esferaTr3 = new PxTransform(PxVec3(ejeZ.getX(), ejeZ.getY(), ejeZ.getZ()));
	RenderItem* rEsfera3 = new RenderItem(esferaShape, esferaTr3, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RegisterRenderItem(rEsfera3);
	_parts.push_back(rEsfera3);

	// SUELO
	PxRigidStatic* Suelo = gPhysics->createRigidStatic(PxTransform({ 50, 0, -80 }));
	
	// Crear material del suelo: poca fricción, muy elástico
	physx::PxMaterial* sueloMat = gPhysics->createMaterial(0.0f, 0.0f, 0.9f); // friction: 0, restitution: 0.9

	// Crear forma del suelo y asignarle el material
	physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(100, 0.1, 100));
	shapeSuelo->setMaterials(&sueloMat, 1);

	//physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(100, 0.1, 100));
	Suelo->attachShape(*shapeSuelo);
	_gScene->addActor(*Suelo);

	// Pintar suelo
	RenderItem* item;
	item = new RenderItem(shapeSuelo, Suelo, { 0.8, 0.8,0.8,1 });

	
	// Anadir un actor dinamico
	PxRigidDynamic* new_solid;
	new_solid = gPhysics->createRigidDynamic(PxTransform({ 50,200,-80 }));
	new_solid->setLinearVelocity({ 0,5,0 });
	new_solid->setAngularVelocity({ 0,0,0 });
	physx::PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
	new_solid->attachShape(*shape_ad);

	PxRigidBodyExt::updateMassAndInertia(*new_solid, 0.15);
	//_gScene->addActor(*new_solid);

	// Pintar actor dinamico
	//RenderItem* dynamic_item;
	//dynamic_item = new RenderItem(shape_ad, new_solid, { 0.8, 0.8,0.8,1 });
	

	_expSys = new ExplosionRigidBodySystem(new_solid, gPhysics, _gScene);

	// añadir
	/*for (auto r : _expSys->getRigidBodies())
	{
		_gScene->addActor(*r);
	}*/
	
}

void Scene3::update(double t)
{
	_expSys->update(t);
}

void Scene3::clear()
{
	// Eliminar eje de coordenadas
	for (auto p : _parts)
	{
		DeregisterRenderItem(p);
	}

	// Eliminar suelo


	delete _expSys; _expSys = nullptr;


}

bool Scene3::handleKey(unsigned char key, const PxTransform& camera)
{
    return false;
}
