#include "UniformalGen.h"
#include <algorithm>
#include "PxShape.h"
#include <iostream>

using namespace physx;

UniformalGen::UniformalGen(int nPart, double prob, Particula* p, PxPhysics* gPhysic) : ParticleGen()
{
	// --- CONFIGURACIoN DE EFECTO FUEGO ---
	_u = std::uniform_real_distribution<double>(-1.0, 1.0);

	// Las particulas salen de un punto o peque zona
	desP = Vector3(0.05, 0.02, 0.05); // leve dispersion horizontal

	// Direccion base: hacia arriba (Y positiva)
	// Variacion aleatoria: leve en X/Z, fuerte en Y
	desVel = Vector3(1.0, 2.0, 1.0);

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
		double prob = _u(_mt) * 0.5 + 0.5;

		if (prob < getProbGen())
		{
			//clonar modelP
			Particula* clonedP = new Particula(_modelP);

			Vector3 randomOffset(_u(_mt), fabs(_u(_mt)), _u(_mt));
			Vector3 newPos = _modelP->getPos() + Vector3(randomOffset.x * desP.x,
				randomOffset.y * desP.y,
				randomOffset.z * desP.z);
			double newDuration = _modelP->getTimeVida() + fabs(_u(_mt)) * desDur;

			double upSpeed = 5.0 + fabs(_u(_mt)) * desVel.y;
			double lateralX = _u(_mt) * desVel.x * 0.5;
			double lateralZ = _u(_mt) * desVel.z * 0.5;

			Vector3 newVel = Vector3(lateralX, upSpeed, lateralZ);
			clonedP->setVel(newVel);

			clonedP->setPos(newPos);
			clonedP->setTimeVida(newDuration);

			float r = std::clamp(0.9f + static_cast<float>(_u(_mt)) * 0.1f, 0.0f, 1.0f);
			float g = std::clamp(0.1f + static_cast<float>(_u(_mt)) * 0.2f, 0.0f, 1.0f);
			float b = std::clamp(0.02f + static_cast<float>(_u(_mt)) * 0.03f, 0.0f, 1.0f);
			float a = 1.0f;

			clonedP->setRenderItem(new RenderItem(esferaShape, clonedP->getTransform(), Vector4(r, g, b, a)));
			auxList.push_back(clonedP);
		}
	}

	
	return auxList;
}