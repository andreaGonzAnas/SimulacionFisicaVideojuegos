#include "GravityForceGenerator.h"
#include <cmath>

using namespace std;

GravityForceGenerator::GravityForceGenerator()
{
	//inicializar atributos
	
}

void GravityForceGenerator::update(Particula* particle, double t)
{
	//acceder a todas las particulas que contengan esta fuerza y actualizarlas
	if (particle->getMasa() <= 0.0f) return; // ignora masas infinitas
	Vector3 fuerza = gravity * particle->getMasa();
	particle->addForce(fuerza);

}

void GravityForceGenerator::updateRigidBody(physx::PxRigidDynamic* b, double t)
{
	if (b->getMass() <= 0.0f) return;
	PxVec3 fuerza = PxVec3(0.0f, -9.81f, 0.0f) * b->getMass(); // gravedad hacia abajo

	b->addForce(fuerza, PxForceMode::eFORCE);
}
