#pragma once

#include <random>
#include "core.hpp"

#include "Particula.h"
#include <list>

#include "PxPhysicsAPI.h"
using namespace physx;

class RigidBodyGen
{
protected:
	int nRigidBodies;
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u{ 0.0,1.0 };
	Vector3 desP;
	Vector3 desVel;
	double desDur;
	double probGeneration;
	Vector4 desColor;

	//masa e inercia
	//friccion y elasticidad

	PxPhysics* gPhysics;
	PxRigidDynamic* _modelRigidBody;

	bool active = true;

	PxScene* _gScene;
	
public:
	RigidBodyGen();
	virtual ~RigidBodyGen() {};
	virtual std::list<PxRigidDynamic*> generateP() = 0;

	//getters y setters
	Vector3 getInitialPos() { return desP; }
	void setInitialPos(Vector3 pos) { desP = pos; }

	Vector3 getVel() { return desP; }
	void setVel(Vector3 vel) { desVel = vel; }

	double getDuration() { return desDur; }
	void setDuration(double duration) { desDur = duration; }

	double getProbGen() { return probGeneration; }
	void setProbGen(double prob) { probGeneration = prob; }

	int getParticulas() { return nRigidBodies; }
	void setParticulas(int n) { nRigidBodies = n; }

	void setDesColor(Vector4 color) { desColor = color; }

	void setModelP(PxRigidDynamic* p) { _modelRigidBody = p; }

	void setActive(bool a) { active = a; }
	bool isActive() const { return active; }



	PxScene* get_gScene()
	{
		return _gScene;
	}

	void set_gScene(PxScene* scene)
	{
		_gScene = scene;
	}

};

