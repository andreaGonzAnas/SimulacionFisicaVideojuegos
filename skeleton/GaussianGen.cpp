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
	desP = Vector3(0.5, 0.5, 0.5);
	desVel = Vector3(0.6, 0.6, 0.6);
	desDur = 10;
	setParticulas(nPart);
	setProbGen(prob);

	//setear particula
	//_modelP = p;

	gPhysics = gPhysic;
}

/*
GaussianGen::~GaussianGen()
{

}*/

/*
std::list<Particula*> GaussianGen::generateP()
{
	std::list<Particula*> auxList;

	for (int i = 0; i < nParticulas; i++)
	{
		double r = _u(_mt);
		if (r < getProbGen())
		{
			// clonar la partícula modelo
			Particula* clonedP = new Particula(_modelP);

			// inicializar prePos para Verlet
			double t = 0.016; // timestep inicial aproximado
			clonedP->setPrePos(clonedP->getPrePos() - clonedP->getVel() * t);

			// asegurar que cada clone tenga su Transform único
			if (!clonedP->getTransform())
				clonedP->setTr(new PxTransform());
			clonedP->getTransform()->p = clonedP->getPrePos();

			// asignar aceleración y tiempo de vida
			clonedP->setAcc(Vector3(0, -9.8, 0));
			clonedP->setTimeVida(_modelP->getTimeVida());

			// crear RenderItem único para la clone
			
			if (_modelP->getRenderItem()) {
				// Suponiendo que tienes acceso a esferaShape y color
				clonedP->setRenderItem(new RenderItem(_Pshape, clonedP->getTransform(), Vector4(1.0f, 0.0f, 1.0f, 1.0f)));

			}

			auxList.push_back(clonedP);
		}
	}

	return auxList;
}*/



std::list<Particula*> GaussianGen::generateP()
{
	
	std::list<Particula*> auxList;

	int cant = 0;

	// Crear geometría y material una sola vez
	static PxSphereGeometry gSphere(1.5f);
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
			Vector3 newPos = _modelP->getPrePos() + _d(_mt) * desP;
			Vector3 newVel = _modelP->getVel() + _d(_mt) * desVel;
			double newDuration = _modelP->getTimeVida() + _d(_mt) * desDur;

			// Inicializar prePos para Verlet
			clonedP->setPos(newPos);
			clonedP->setVel(newVel);
			clonedP->setTimeVida(newDuration);
			clonedP->setAcc(Vector3(0, -9.8, 0));

			// RenderItem único
			clonedP->setRenderItem(new RenderItem(esferaShape, clonedP->getTransform(), Vector4(1, 0, 0, 1)));

			auxList.push_back(clonedP);
			cant++;
		}
	}
	std::cout << "Cantidad de partículas creadas: " << cant << std::endl;
	return auxList;
}
