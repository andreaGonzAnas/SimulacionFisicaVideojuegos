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
	if (particle->getMasa() <= 0.0f) return;

	//la velocidad del viento cambia en funcion del tiempo
	double oscillation = sin(t * 0.5) * 0.5 + 1.0;
	Vector3 windRealVel = windVel * oscillation;

	Vector3 diff = windRealVel - particle->getVel();
	double module = diff.magnitude();

	// Evitar aplicar fuerza si la velocidad ya es casi la del viento
	if (module < 0.1) return;

	Vector3 windForce = k1 * diff + k2 * module * diff;
	particle->addForce(windForce);


}
