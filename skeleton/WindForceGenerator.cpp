#include "WindForceGenerator.h"

#include <cmath>

WindForceGenerator::WindForceGenerator(Vector3 wind, double k1, double k2): windVel(wind), k1(k1), k2(k2)
{
	
}

void WindForceGenerator::update(Particula* particle, double t)
{
	//FORMULA
	//𝐹𝑣⃗⃗⃗ = 𝑘1(V𝑣 − 𝑣) + 𝑘2 * ‖V𝑣 − 𝑣 ‖ * (V𝑣 − 𝑣)
	
	//acceder a todas las particulas que contengan esta fuerza y actualizarlas
	if (particle->getMasa() <= 0.0f) return; // ignora masas infinitas
	double module = (windVel - particle->getVel()).magnitude();
	
	Vector3 windForce = k1 * (windVel - particle->getVel()) 
		+ (k2 * module * (windVel - particle->getVel()));

	particle->addForce(windForce);

}
