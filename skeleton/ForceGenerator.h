#pragma once
#include "ParticleGen.h"
#include <iostream>

class ForceGenerator
{
protected:
	bool active = true;
public:
	ForceGenerator() {};
	virtual void update(Particula* particle, double t) = 0;

	void setActive(bool a) { 
		std::cout << "Active: " << active << '\n';
		active = a; }
	bool isActive() const { return active; }
};

