#pragma once

#include "RenderUtils.hpp"

class Particula
{
private:
	RenderItem* renderItem;
	physx::PxTransform* tr;
	Vector3 vel;
	Vector3 acceleration;
	double damping;
	double masa;
	double tVida; //tiempo de vida

public:
	Particula();
	Particula(Vector3 pos, Vector3 vel, Vector3 a, double damping);
	~Particula(); //destructora

	//gets
	physx::PxTransform* getTr();
	RenderItem* getRenderItem();
	Vector3 getVel();
	Vector3 getAcc();
	double getMasa();
	double getTimeVida();

	//sets
	void setTr(physx::PxTransform*);
	void setRenderItem(RenderItem*);
	void setVel(Vector3 vel);
	void setAcc(Vector3 a);
	void setMasa(double masa);
	void setTimeVida(double t);

	//actualizar posicion de la particula
	void integrate(double t);

	//damping entre 0 y 1. Creo q usaremos 0.98

};

