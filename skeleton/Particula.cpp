#include "Particula.h"
using namespace physx;

Particula::Particula()
{
}

Particula::Particula(Vector3 pos, Vector3 vel, Vector3 a, double d) : vel(vel), acceleration(a), damping(d), firstFrame(true)
{
    //atributos a cambiar
    masa = 1.0;
    tVida = 10.0;

    tr = new PxTransform(pos);
    prePos = pos - vel;
}

Particula::Particula(Vector3 pos, Vector3 vel, Vector3 a, double damping, double m) : vel(vel), acceleration(a), damping(damping), masa(m), firstFrame(true)
{
    //atributos a cambiar
    tVida = 10.0;

    tr = new PxTransform(pos);
    prePos = pos - vel;
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
    
    if (tVida > 0.0 && masa > 0.0)
    {
        tVida -= t;

        //euler semi-implicito
        //vel = (vel * pow(damping, t)) + acceleration * t;
        //tr->p = actualPos + t * vel;


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
    
}


