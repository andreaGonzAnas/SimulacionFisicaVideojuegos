#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos): SpringForceGenerator(k, resting, nullptr)
{
	//revisar esto
	_other = new Particula(anchor_pos, { 0,0,0 }, 0, 1e6);
}

AnchoredSpringFG::~AnchoredSpringFG()
{
}
