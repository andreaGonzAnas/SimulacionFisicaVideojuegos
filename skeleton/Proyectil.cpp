#include "Proyectil.h"
#include "Particula.h"
#include "PxShape.h"

Proyectil::Proyectil(Vector4 color, double masaR, double velR, double velS, physx::PxShape* esferaShape)
{
	//PARA PROYECTIL
	double energiaR = 0.5 * masaR * velR * velR;

	//energia simulada
	double energiaS = energiaR;
	//double velS = 25; // m/s velocidad simulada (la que quiero utilizar)

	double masaS = masaR * pow((velR / velS), 2); //masa simulada

	Vector3 pos = GetCamera()->getTransform().p; // pos inicial cañon = pos de la camara
	Vector3 vel = GetCamera()->getDir() * velS; //velocidad inicial: la direccion es la de la camara * velocidad simulada


	p = new Particula(pos, vel, 0.98, masaS);
	RenderItem* renderItem = new RenderItem(esferaShape, p->getTr(), color);
	p->setRenderItem(renderItem);

}
