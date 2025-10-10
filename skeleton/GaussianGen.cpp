#include "GaussianGen.h"


GaussianGen::GaussianGen()
{
	//setear desviaciones

}

GaussianGen::~GaussianGen()
{

}

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
			Particula* clonedP = _modelP;

			//calcular nuevas propiedades
			//cambiar p con lo nuevo calculado

			//posicion
			Vector3 newPos = Vector3(0,0,0) + _d(_mt) * desP;
			Vector3 newVel = Vector3(0,0,0) + _d(_mt) * desV;
			Vector3 newDuration = Vector3(0,0,0) + _d(_mt) * desDur;
			
			auxList.push_back(clonedP);
		}
	}

	return auxList;
}
