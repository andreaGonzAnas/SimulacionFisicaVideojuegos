#pragma once
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator
{
private:
    //atributos de la fuerza

    //fuerza fuerte
    Vector3 windVel;
    double coef; //coeficiente de rozamiento?
    double k1; //??
    double k2;

public:
    //Constructora
    WindForceGenerator(Vector3 windVel, double k1, double k2);

    //Update
    void update(Particula* particle, double t) override;
};

