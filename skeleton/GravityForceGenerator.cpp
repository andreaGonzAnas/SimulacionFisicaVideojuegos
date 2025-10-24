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
