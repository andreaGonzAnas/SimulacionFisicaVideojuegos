#pragma once
#include "ForceGenerator.h"
class WhirlwindForceGenerator : public ForceGenerator
{
private:
    Vector3 origin;
    double strength;
    double radius;
    double k1;

public:
    //Constructora
    WhirlwindForceGenerator(Vector3 o, double k1, double s, double r);
    
    //Update
    void update(Particula* particle, double t) override;
};

