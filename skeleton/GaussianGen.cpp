#include "GaussianGen.h"
#include "PxShape.h"
#include <iostream>

using namespace physx;

GaussianGen::GaussianGen()
{
}

GaussianGen::GaussianGen(int nPart, double prob, Particula* p, PxPhysics* gPhysic) : ParticleGen(), _d(0.0, 1.0), _modelP(p)
{
	//setear desviaciones
	desP = Vector3(0, 0, 0);
	desVel = Vector3(10, 10, 10);
	desDur = 10;
	setParticulas(nPart);
	setProbGen(prob);

	desColor = Vector4(0.2, 0.2, 0.1, 1.0);


	gPhysics = gPhysic;
}

/*
GaussianGen::~GaussianGen()
{

}*/

std::list<Particula*> GaussianGen::generateP()
{
	
	std::list<Particula*> auxList;

	// Crear geometría y material una sola vez
	static PxSphereGeometry gSphere(0.5f);
	static PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	static physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);


	for (int i = 0; i < nParticulas; i++)
	{
		//comprobar probabilidad para crear o no particulas
		double r = _u(_mt); // numero aleatorio entre 0 y 1

		if (r < getProbGen())
		{
			//clonar modelP
			Particula* clonedP = new Particula(_modelP);

			// Variación gaussiana de atributos
			Vector3 newPos = _modelP->getPos() + _d(_mt) * desP;
			///Vector3 newVel = _modelP->getVel() + _d(_mt) * desVel;
			double newDuration = _modelP->getTimeVida() + _d(_mt) * desDur;

			// --- Velocidad radial ---
			Vector3 dirVel(_d(_mt), _d(_mt), _d(_mt));   // vector aleatorio 3D
			if (dirVel.magnitude() > 0.0)
				dirVel = dirVel.getNormalized();            // normalizar para dirección
			double speed = 40; // magnitud aleatoria
			Vector3 newVel = dirVel * speed;
			clonedP->setVel(newVel);

			// Inicializar prePos para Verlet
			clonedP->setPos(newPos);
			clonedP->setVel(newVel);
			clonedP->setTimeVida(newDuration);
			clonedP->setAcc(Vector3(0, -9.8, 0));

			float r = clonedP->getColor().x + (_d(_mt) * desColor.x);
			float g = clonedP->getColor().y + (_d(_mt) * desColor.y);
			float b = clonedP->getColor().z + (_d(_mt) * desColor.z);
			float a = 1.0f; // opacidad total

			// RenderItem único
			clonedP->setRenderItem(new RenderItem(esferaShape, clonedP->getTransform(), Vector4(r, g, b, a)));

			auxList.push_back(clonedP);
		}
	}
	return auxList;
}
