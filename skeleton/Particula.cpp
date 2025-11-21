#include "Particula.h"
#include <iostream>


using namespace physx;

Particula::Particula()
{
}

Particula::Particula(Vector3 pos, Vector3 vel, double d) : vel(vel), damping(d), firstFrame(true)
{
    //atributos a cambiar
    masa = 1.0;
    tVida = 0.5;

    tr = new PxTransform(pos);
    prePos = pos - vel;
    acceleration = Vector3(0, 0, 0);
    forceAccum = Vector3(0, 0, 0);
}

Particula::Particula(Vector3 pos, Vector3 vel, double damping, double m) : vel(vel), damping(damping), masa(m), firstFrame(true)
{
    //atributos a cambiar
    tVida = 5.0;

    tr = new PxTransform(pos);
    prePos = pos - vel;
    acceleration = Vector3(0, 0, 0);
    forceAccum = Vector3(0, 0, 0);
}

Particula::Particula(Particula* p) : vel(p->getVel()), acceleration(p->getAcc()), damping(p->getDamping()), firstFrame(true),
masa(p->getMasa()), tVida(p->getTimeVida()), color(p->getColor()), forceAccum(Vector3(0, 0, 0))
{
    // Clonar el transform si existe
        if (p->getTransform())
            tr = new PxTransform(*p->getTransform());
        else
            tr = new PxTransform();

    prePos = tr->p - vel;
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

double Particula::getDamping()
{
    return damping;
}

physx::PxTransform* Particula::getTransform()
{
    return tr;
}

Vector3 Particula::getPrePos()
{
    return prePos;
}

Vector3 Particula::getPos()
{
    return tr->p;
}

Vector4 Particula::getColor()
{
    return color;
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

void Particula::setPrePos(Vector3 pos)
{
    prePos = pos;
}

void Particula::setPos(Vector3 pos)
{
    tr->p = pos;
}

void Particula::setColor(Vector4 c)
{
    color = c;
}

void Particula::setTime(double t)
{
    tVida = t;
}

void Particula::integrate(double t)
{
    Vector3 actualPos = tr->p;
    
    acceleration = forceAccum / masa;

    if (tVida > 0.0 && masa > 0.0)
    {
        tVida -= t;

        if (firstFrame) {
            // euler semi-implicito para el primer paso
            vel = (vel * pow(damping, t)) + acceleration * t;
            tr->p = actualPos + t * vel;
            prePos = actualPos;
            firstFrame = false;
        }
        else
        {
            //verlet
            Vector3 newPos = actualPos + (actualPos - prePos) * damping + acceleration * (t * t);
            prePos = actualPos;
            tr->p = newPos;
        }
        
    }

    clearForces();
    
}

//añadir fuerza a una particula


void Particula::addForce(physx::PxVec3 force)
{
    // Acumular fuerzas (suma de todas las que le afectan)
    forceAccum += Vector3(force.x, force.y, force.z);
}

//quitar la fuerza de la particula
void Particula::clearForces()
{
    forceAccum = Vector3(0, 0, 0);
}


