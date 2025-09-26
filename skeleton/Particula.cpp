#include "Particula.h"
using namespace physx;

Particula::Particula()
{
}

Particula::Particula(Vector3 pos, Vector3 vel, Vector3 a): vel(vel), acceleration(a)
{
    //atributos a cambiar
    masa = 5;
    tVida = 5;

    tr = new PxTransform(pos);

    
}

Particula::~Particula()
{
    // deregistrar el objeto RenderItem de la escena
    DeregisterRenderItem(renderItem);
}

physx::PxTransform* Particula::getTr()
{
    return tr;
}

RenderItem* Particula::getRenderItem()
{
    return renderItem;
}

Vector3 Particula::getVel()
{
    return vel;
}

Vector3 Particula::getAcc()
{
    return acceleration;
}

double Particula::getMasa()
{
    return masa;
}

double Particula::getTimeVida()
{
    return tVida;
}

void Particula::setTr(physx::PxTransform* newTr)
{
    tr = newTr;
}

void Particula::setRenderItem(RenderItem* rI)
{
    renderItem = rI;
    RegisterRenderItem(renderItem); //y registrar item a renderizar
}

void Particula::setVel(Vector3 v)
{
    vel = v;
}

void Particula::setAcc(Vector3 a)
{
    acceleration = a;
}

void Particula::setMasa(double m)
{
    masa = m;
}

void Particula::setTimeVida(double t)
{
    tVida = t;
}

void Particula::integrate(double t)
{
    Vector3 actualPos = tr->p;

    if (masa <= 0.0) return;

    //vel = vel + acceleration * t;

    tr->p = actualPos + t * vel;
    
}
