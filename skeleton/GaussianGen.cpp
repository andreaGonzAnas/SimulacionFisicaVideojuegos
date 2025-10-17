#include "GaussianGen.h"

using namespace physx;

GaussianGen::GaussianGen()
{
}

GaussianGen::GaussianGen(int nPart, double prob, Particula* p): _modelP(nullptr)
{
	//setear desviaciones
	desP = Vector3(0.1, 0.1, 0.1);
	desVel = Vector3(0.05, 0.05, 0.05);
	desDur = 0.2;
	setParticulas(nPart);
	setProbGen(prob);

	//setear particula
	_modelP = p;
}

/*
GaussianGen::~GaussianGen()
{

}*/

std::list<Particula*> GaussianGen::generateP()
{
	std::list<Particula*> auxList;

	//comprobar probabilidad para crear o no particulas
	double r = _u(_mt); // numero aleatorio entre 0 y 1

	if (r < getProbGen())
	{
		for (int i = 0; i < nParticulas; i++)
		{
			//clonar modelP
			Particula* clonedP = new Particula(*_modelP);

			//las propiedades siguientes varian dependiendo de x prob

			//posicion
			Vector3 newPos = Vector3(0,0,0) + _d(_mt) * desP;
			Vector3 newVel = Vector3(0,0,0) + _d(_mt) * desVel;
			double newDuration = 0.0 + _d(_mt) * desDur;

			clonedP->setPrePos(newPos);
			clonedP->setVel(newVel);
			clonedP->setTimeVida(newDuration);
			
			auxList.push_back(clonedP);
		}
	}

	return auxList;
}
