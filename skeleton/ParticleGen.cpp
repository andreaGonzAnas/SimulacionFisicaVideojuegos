#include "ParticleGen.h"

ParticleGen::ParticleGen() : _mt(std::random_device{}()), _u(0.0, 1.0)
{

}
