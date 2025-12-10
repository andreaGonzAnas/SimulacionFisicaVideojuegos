#include "UniformalRigidBodyGen.h"
#include "DynamicObj.h"

UniformalRigidBodyGen::UniformalRigidBodyGen(int nPart, double prob, PxRigidDynamic* p, PxPhysics* gPhysic)
{
	_mt = std::mt19937(std::random_device{}());
	_u = std::uniform_real_distribution<double>(-1.0, 1.0);

	// Configuracion general
	setParticulas(nPart);
	setProbGen(prob);

	_modelRigidBody = p;
	gPhysics = gPhysic;

}

UniformalRigidBodyGen::~UniformalRigidBodyGen()
{
}

std::list<DynamicObj*> UniformalRigidBodyGen::generateP()
{
	std::list<DynamicObj*> auxList;

	if (!active) return auxList;

	physx::PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));

	for (int i = 0; i < nRigidBodies; i++)
	{
		if (_u(_mt) * 0.5 + 0.5 < getProbGen())
		{
			// Crear objeto dinamico
			DynamicObj* _dynamicObj = new DynamicObj({ 0,5,0 }, { 0,0,0 }, shape_ad, { 50,200,-80 }, 0.15, 5.0, gPhysics);
			
			/*
			//crear rigid body dinamico
			PxRigidDynamic* new_solid;
			new_solid = gPhysics->createRigidDynamic(PxTransform({ 50,200,-80 }));
			new_solid->setLinearVelocity({ 0,5,0 });
			new_solid->setAngularVelocity({ 0,0,0 });
			new_solid->attachShape(*shape_ad);

			PxRigidBodyExt::updateMassAndInertia(*new_solid, 0.15);
			//_gScene->addActor(*new_solid); // hacer esto fuera

			// Pintar actor dinamico
			RenderItem* dynamic_item;
			dynamic_item = new RenderItem(shape_ad, new_solid, { 0.8, 0.8,0.8,1 });
			*/

			//clonedP->setRenderItem(new RenderItem(esferaShape, clonedP->getTransform(), newC));
			auxList.push_back(_dynamicObj);

		}
	}

	return auxList;
}
