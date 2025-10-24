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
	Vector3 prePos;
	bool firstFrame;
	Vector4 color;

	Vector3 forceAccum; // fuerzas acumuladas
	double gravity = -9.8f; // fuerzas acumuladas

public:
	Particula();
	Particula(Vector3 pos, Vector3 vel, double damping); //para particula
	Particula(Vector3 pos, Vector3 vel, double damping, double masa); //para proyectil
	Particula(Particula* p);
	~Particula(); //destructora

	//gets
	physx::PxTransform* getTr();
	RenderItem* getRenderItem();
	Vector3 getVel();
	Vector3 getAcc();
	double getMasa();
	double getTimeVida();
	double getDamping();
	physx::PxTransform* getTransform();
	Vector3 getPrePos();
	Vector3 getPos();
	Vector4 getColor();

	//sets
	void setTr(physx::PxTransform*);
	void setRenderItem(RenderItem*);
	void setVel(Vector3 vel);
	void setAcc(Vector3 a);
	void setMasa(double masa);
	void setTimeVida(double t);
	void setPrePos(Vector3 pos);
	void setPos(Vector3 pos);
	void setColor(Vector4 color);

	//actualizar posicion de la particula
	void integrate(double t);

	//damping entre 0 y 1. Creo q usaremos 0.98

	//Fuerzas
	void addForce(physx::PxVec3 force);
	void clearForces();
};

