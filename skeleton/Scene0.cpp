#include "Scene0.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"

Scene0::Scene0(PxPhysics* physics): Scene(physics), _firePartSystem(nullptr), _confettiPartSys(nullptr), _fireworkPartSys(nullptr), _proyectilSys(nullptr)
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
}

void Scene0::update(double t)
{
}

void Scene0::clear()
{
}
