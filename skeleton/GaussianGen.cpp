#include "GaussianGen.h"

using namespace physx;

GaussianGen::GaussianGen()
{
}

GaussianGen::GaussianGen(int nPart, double prob, Particula* p): _modelP(nullptr)
{
	//setear desviaciones
	desP = Vector3(0.5, 0.5, 0.5);
	desVel = Vector3(0.6, 0.6, 0.6);
	desDur = 10;
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

	for (int i = 0; i < nParticulas; i++)
	{
		if (r < getProbGen())
		{
			//clonar modelP
			Particula* clonedP = new Particula(*_modelP);

			//las propiedades siguientes varian dependiendo de x prob

			//posicion
			Vector3 newPos = clonedP->getPrePos() + _d(_mt) * desP;
			Vector3 newVel = clonedP->getVel() + _d(_mt) * desVel;
			double newDuration = clonedP->getTimeVida() + _d(_mt) * desDur;

			clonedP->setPrePos(newPos);
			clonedP->setVel(newVel * 50);
			clonedP->setTimeVida(newDuration);
			clonedP->setAcc(Vector3(0, -9.8, 0));

			auxList.push_back(clonedP);
		}
	}

	return auxList;
}
