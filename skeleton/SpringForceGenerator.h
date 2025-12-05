#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator
{
protected:
	double _k;
	double _resting_length;
	Particula* _other;
	Vector3 _anchored_pos;

public:
	SpringForceGenerator(double k, double resting_length, Particula* other);
	void update(Particula* particle, double t) override;
	inline void setK(double k) { _k = k; }
	double getK() { return _k; }
	virtual ~SpringForceGenerator() {}
};

