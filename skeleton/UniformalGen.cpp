#include "UniformalGen.h"
#include <algorithm>
#include "PxShape.h"
#include <iostream>
#include <algorithm>

using namespace physx;

UniformalGen::UniformalGen(int nPart, double prob, Particula* p, PxPhysics* gPhysic) : ParticleGen()
{
	// --- CONFIGURACIoN DE EFECTO FUEGO ---
	_mt = std::mt19937(std::random_device{}());
	_u = std::uniform_real_distribution<double>(-1.0, 1.0);

	// Las particulas salen de un punto o peque zona
	desP = Vector3(1.5, 0.02, 1.5); // leve dispersion horizontal

	// Direccion base: hacia arriba (Y positiva)
	// Variacion aleatoria: leve en X/Z, fuerte en Y
	desVel = Vector3(0.3, 0.1, 0.3);

	// Vida corta: las llamas duran poco
	desDur = 0.2; // segundos aprox

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

	static PxSphereGeometry gSphere(0.2f);
	static PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	static physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	for (int i = 0; i < nParticulas; i++)
	{
		if (_u(_mt) * 0.5 + 0.5 < getProbGen())
		{
			Particula* clonedP = new Particula(_modelP);

			// pequeñas variaciones respecto a la partícula modelo
			Vector3 randomOffset(
				_u(_mt) * desP.x,
				fabs(_u(_mt)) * desP.y,
				_u(_mt) * desP.z
			);

			Vector3 newPos = _modelP->getPos() + randomOffset;

			Vector3 newVel = _modelP->getVel() + Vector3(
				_modelP->getVel().x * desVel.x,
				_modelP->getVel().y * desVel.y,
				_modelP->getVel().z * desVel.z
			);

			double newDuration = std::max<double>(0.0, _modelP->getTimeVida() + _u(_mt) * desDur);

			clonedP->setPos(newPos);
			clonedP->setVel(newVel);
			clonedP->setTimeVida(newDuration);

			Vector4 baseC = _modelP->getColor();
			Vector4 newC = Vector4(
				std::clamp(baseC.x + static_cast<float>(_u(_mt)) * desColor.x, 0.0f, 1.0f),
				std::clamp(baseC.y + static_cast<float>(_u(_mt)) * desColor.y, 0.0f, 1.0f),
				std::clamp(baseC.z + static_cast<float>(_u(_mt)) * desColor.z, 0.0f, 1.0f),
				baseC.w
			);

			clonedP->setRenderItem(new RenderItem(esferaShape, clonedP->getTransform(), newC));
			auxList.push_back(clonedP);
		}
	}

	return auxList;
}