#include "UniformalGen.h"
#include <algorithm>
#include "PxShape.h"
#include <iostream>

using namespace physx;

UniformalGen::UniformalGen(int nPart, double prob, Particula* p, PxPhysics* gPhysic) : ParticleGen(), _d(0.0, 1.0)
{
	// --- CONFIGURACIoN DE EFECTO FUEGO ---

	// Las particulas salen de un punto o peque zona
	desP = Vector3(0.3, 0.1, 0.3); // leve dispersion horizontal

	// Direccion base: hacia arriba (Y positiva)
	// Variacion aleatoria: leve en X/Z, fuerte en Y
	desVel = Vector3(8.0, 10.0, 8.0);

	// Vida corta: las llamas duran poco
	desDur = 0; // segundos aprox

	// Colores calidos: entre rojo, naranja y amarillo
	// (se usaron como desviaciones al generar el color)
	desColor = Vector4(0.2f, 0.2f, 0.05f, 1.0f);

	// Configuracion general
	setParticulas(nPart);
	setProbGen(prob);

	_modelP = p;
	gPhysics = gPhysic;
}

UniformalGen::~UniformalGen()
{
}

std::list<Particula*> UniformalGen::generateP()
{
	std::list<Particula*> auxList;

	// Crear geometría y material
	static PxSphereGeometry gSphere(0.2f);
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
			double newDuration = _modelP->getTimeVida() + _d(_mt) * desDur;

			//Velocidad tipo fuego
			double upSpeed = 5.0 + fabs(_d(_mt)) * desVel.y;  // siempre positivo
			double lateralX = _d(_mt) * desVel.x * 0.3;
			double lateralZ = _d(_mt) * desVel.z * 0.3;

			Vector3 newVel = Vector3(lateralX, upSpeed, lateralZ);
			clonedP->setVel(newVel);

			clonedP->setPos(newPos);
			clonedP->setTimeVida(newDuration);

			// Limitar para que solo sean rojo-naranja-amarillo
			float r = std::clamp(static_cast<float>(r), 0.9f, 1.0f);
			float g = std::clamp(static_cast<float>(g), 0.0f, 0.3f);
			float b = std::clamp(static_cast<float>(b), 0.0f, 0.05f);

			float a = 1.0f; // opacidad total

			// RenderItem único
			clonedP->setRenderItem(new RenderItem(esferaShape, clonedP->getTransform(), Vector4(r, g, b, a)));
			auxList.push_back(clonedP);
		}
	}

	
	return auxList;
}