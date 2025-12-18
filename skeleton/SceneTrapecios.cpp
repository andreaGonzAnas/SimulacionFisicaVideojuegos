#include "SceneTrapecios.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "MyContactCallback.h"
#include <iostream>

#include "CollectibleParticleSystem.h"

SceneTrapecios::SceneTrapecios(PxPhysics* physics, PxScene* scene) : Scene(physics)
{
    set_gScene(scene);
}

SceneTrapecios::~SceneTrapecios()
{
}

void SceneTrapecios::init()
{
	// CALLBACK
    _myCallback = new MyContactCallback(this);
    
    // ---- DECORACION ----
	createDeco();

    // ---- CAMERA ----
    PxVec3 centro(35, 40, 35);
    Camera* cam = GetCamera();
    _initPosCamera = cam->getEye();
    _initDirCamera = cam->getDir();
    cam->setTransform(PxVec3(centro.x, centro.y - 10.0, centro.z - 45.0));
    cam->setDir(PxVec3(0.0, 0.13, 1));
    cam->setHumanCannonMode(true);

	// ---- JOINTS/TRAPECIOS ----
    createTrapecio(PxVec3(50, 53.5, 35), false);

    createTrapecio(PxVec3(20, 53.5, 35), true);

    // ---- PLATAFORMAS ----
    createPlatforms(PxVec3(65, 33.5, 35));
    createPlatforms(PxVec3(5, 33.5, 35));

	// ---- PLAYER ----
    createPlayer(20.0f);

	// ---- SUELO (CAMA ELASTICA) ----
    createMalla();

    // ---- PARTICULA RECOGIBLE ----
    //Material
    PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    _springSys = new CollectibleParticleSystem(gMaterial, physx::PxVec3(35, 53.5, 35));
    _staticParticle = _springSys->getStaticPart();
}

void SceneTrapecios::update(double t)
{
    // si player tiene que ponerse en trapecio...
    if (_pendingAttachRegistered && _pendingAttachPalo) {
        attachPlayerToTrapecio(_pendingAttachPalo);
        _pendingAttachRegistered = false;
        _pendingAttachPalo = nullptr;
    }
    
    for (auto& t : _trapecios)
    {
        if (!t.active) continue;

        PxReal angle = t.joint->getAngle();

        if (angle >= PxPi / 4) t.motorVel = -fabs(t.motorVel);
        else if (angle <= -PxPi / 4) t.motorVel = fabs(t.motorVel);

        t.joint->setDriveVelocity(t.motorVel);
        t.palo2->wakeUp(); // asegura que el actor dinámico esté despierto
    }

    

    checkPlayerCollectible();

    if (_hasCollectedParticle && _staticParticle && _player)
    {
        PxVec3 playerPos = _player->getGlobalPose().p;

        PxVec3 offset(0.0f, 1.0f, 0.0f); // ejemplo
        _springSys->setStaticPos(playerPos + offset);
    }

    _springSys->update(t);
}

void SceneTrapecios::clear()
{
    // CALLBACK
    if (_myCallback)
    {
        _gScene->setSimulationEventCallback(nullptr);
        delete _myCallback;
        _myCallback = nullptr;
    }

    
    //// Muelle
    //delete _springSys; _springSys = nullptr;
    //_staticParticle = nullptr;
    //
    //// eliminar statics

    //for (auto* suelo : _statics)
    //{
    //    if (!suelo) continue;

    //    // Quitar del escenario
    //    _gScene->removeActor(*suelo);

    //    // Liberar shapes
    //    PxU32 nShapes = suelo->getNbShapes();
    //    std::vector<physx::PxShape*> shapes(nShapes);
    //    suelo->getShapes(shapes.data(), nShapes);

    //    for (physx::PxShape* shape : shapes)
    //    {
    //        if (shape) shape->release();
    //    }

    //    // Liberar el actor
    //    suelo->release();
    //}

    //// Vaciar el vector y evitar punteros colgantes
    //_statics.clear();
    //
    ////Eliminar rigidBodies
    //for (auto r : _rigids)
    //{
    //    DeregisterRenderItem(r->getRenderItem());

    //    // Eliminar suelo
    //    if (r->getRigidDynamic()) {
    //        _gScene->removeActor(*r->getRigidDynamic());
    //    }

    //    if (r->getRigidDynamic()) {
    //        PxU32 nShapes = r->getRigidDynamic()->getNbShapes();
    //        PxShape* shapes[8];
    //        r->getRigidDynamic()->getShapes(shapes, nShapes);

    //        for (PxU32 i = 0; i < nShapes; i++) {
    //            shapes[i]->release();
    //        }
    //    }

    //    // 3. Liberar el actor estático
    //    if (r->getRigidDynamic()) {
    //        r->getRigidDynamic()->release();
    //        delete r;
    //    }
    //}
    

}

bool SceneTrapecios::handleKey(unsigned char key, const PxTransform& camera)
{
    switch (key)
    {
        case 't':
        {
            if (!_trapecios.empty())
            {
                _trapecios[0].active = true;                // activa la lógica del motor
                _trapecios[0].motorVel = 1.0f;              // asegura velocidad inicial positiva
                _trapecios[0].palo2->wakeUp();              // despierta el actor dinámico
                _trapecios[0].joint->setDriveVelocity(_trapecios[0].motorVel); // aplica velocidad al motor
                
            }

            break;
        }
        case ' ': // Espacio
        {
            if (_playerJoint)
            {
                // 1. Identificamos el trapecio para reactivar el motor
                for (auto& trap : _trapecios)
                {
                    PxRigidActor* actor1;
                    PxRigidActor* actor2;
                    _playerJoint->getActors(actor1, actor2);

                    if (actor1 == trap.palo2 || actor2 == trap.palo2)
                    {
                        trap.joint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
                        trap.active = true;
                    }
                }

                // 2. Soltamos el joint pero NO reseteamos la velocidad a (0,0,0)
                _playerJoint->release();
                _playerJoint = nullptr;

                // 3. Capturamos la velocidad que traía del balanceo
                PxVec3 velocidadActual = _player->getLinearVelocity();

                // 4. Aplicamos el impulso de salto SUMADO a la inercia
                // Nota: Bajamos un poco el valor del impulso (ej. 3000) porque ya lleva velocidad
                PxVec3 fuerzaSalto(-250.0f, 500.0f, 0.0f);
                _player->addForce(fuerzaSalto, PxForceMode::eIMPULSE);

                _hasJumped = true;
            }
            else if (!_hasJumped) // Salto normal desde el suelo
            {
                PxVec3 impulsoSuelo(-1000.0f, 4200.0f, 0.0f);
                _player->addForce(impulsoSuelo, PxForceMode::eIMPULSE);
                _hasJumped = true;
            }

            break;
        }

        default: return false;
    }
    return true;
}

void SceneTrapecios::createDeco()
{
    //Material
    PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    // SUELO
    PxBoxGeometry gBox = PxBoxGeometry(60.0f, 1.0f, 50.0f);
    physx::PxShape* squareShape = CreateShape(gBox, gMaterial);
    PxTransform* boxTr = new PxTransform(PxVec3(35, 10, 30));
    RenderItem* rBox = new RenderItem(squareShape, boxTr, Vector4(0.8f, 0.30f, 0.11f, 1.0f));
    RegisterRenderItem(rBox); //y registrar item a renderizar
    _scenary.push_back(rBox);

    // PALO IZQUIERDO
    PxBoxGeometry gPaloIzq = PxBoxGeometry(1.0f, 36.0f, 2.0f);
    physx::PxShape* paloShapeIzq = CreateShape(gPaloIzq, gMaterial);
    PxTransform* paloTrIzq = new PxTransform(PxVec3(0, 20, 35));
    RenderItem* rPaloIzq = new RenderItem(paloShapeIzq, paloTrIzq, Vector4(0.6f, 0.6f, 0.6f, 0.2f));
    RegisterRenderItem(rPaloIzq);
    _scenary.push_back(rPaloIzq);

    // PALO DERECHO
    PxBoxGeometry gPaloDer = PxBoxGeometry(1.4f, 36.0f, 2.0f);
    physx::PxShape* paloShapeDer = CreateShape(gPaloDer, gMaterial);
    PxTransform* paloTrDer = new PxTransform(PxVec3(70, 20, 35));
    RenderItem* rPaloDer = new RenderItem(paloShapeDer, paloTrDer, Vector4(0.6f, 0.6f, 0.6f, 0.2f));
    RegisterRenderItem(rPaloDer);
    _scenary.push_back(rPaloDer);

    // PALO SUPERIOR
    PxBoxGeometry gPaloSup = PxBoxGeometry(36.0f, 0.4f, 2.0f);
    physx::PxShape* paloShapeSup = CreateShape(gPaloSup, gMaterial);
    PxTransform* paloTrSup = new PxTransform(PxVec3(35, 54, 35));
    RenderItem* rPaloSup = new RenderItem(paloShapeSup, paloTrSup, Vector4(0.6f, 0.6f, 0.6f, 0.2f));
    RegisterRenderItem(rPaloSup);
    _scenary.push_back(rPaloSup);



}

void SceneTrapecios::createTrapecio(physx::PxVec3 pos, bool startActive)
{
    // =========================
    // PALO FIJO (SOPORTE)
    // =========================
    PxRigidStatic* palo1 = gPhysics->createRigidStatic(
        PxTransform(pos)
    );

    physx::PxShape* shape = CreateShape(PxBoxGeometry(1.0f, 1.0f, 1.0f));
    palo1->attachShape(*shape);

    RenderItem* item = new RenderItem(shape, palo1, { 1.0f, 0.1f, 0.1f, 1.0f });
    _gScene->addActor(*palo1);

    _statics.push_back(palo1);

    // =========================
    // PALO MÓVIL (PÉNDULO CORTO)
    // =========================
    float halfHeight = 7.5f; // altura del péndulo
    PxVec3 posMovil = pos + PxVec3(0.0f, -(halfHeight + 1.0f), 0.0f);

    PxRigidDynamic* palo2 = gPhysics->createRigidDynamic(
        PxTransform(posMovil)
    );

    physx::PxShape* shape2 = CreateShape(PxBoxGeometry(0.5f, halfHeight, 0.5f));
    palo2->attachShape(*shape2);

    PxRigidBodyExt::updateMassAndInertia(*palo2, 40.0f);


    RenderItem* item2 = new RenderItem(shape2, palo2, { 0.0f, 0.1f, 0.8f, 1.0f });
    _gScene->addActor(*palo2);

    _rigids.push_back(palo2);

    struct TrapecioTag { int index; };
    palo2->userData = new TrapecioTag{ (int)_trapecios.size() };


    // =========================
    // JOINT REVOLUTE (EJE X)
    // =========================
    PxRevoluteJoint* art = PxRevoluteJointCreate(
        *gPhysics,
        palo1,
        PxTransform(PxVec3(0, 0, 0), PxQuat(PxHalfPi, PxVec3(0, 1, 0))),
        palo2,
        PxTransform(PxVec3(0, halfHeight, 0), PxQuat(PxHalfPi, PxVec3(0, 1, 0)))
    );
    art->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true); // motor habilitado

    // =========================
    // LÍMITES ANGULARES
    // =========================
    art->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
    art->setLimit(PxJointAngularLimitPair(-PxPi / 4, PxPi / 4, 0.01f));

    // =========================
    // MOTOR
    // =========================
    float initialMotorVel = 1.0f;
    art->setDriveForceLimit(10000.0f);
    art->setDriveVelocity(initialMotorVel);

    // =========================
    // CREAR EL STRUCT TRAPECIO
    // =========================
    _trapecios.emplace_back();          // construye un Trapecio vacío

    _trapecios.back().palo1 = palo1;
    _trapecios.back().palo2 = palo2;
    _trapecios.back().joint = art;
    _trapecios.back().motorVel = initialMotorVel;
    _trapecios.back().active = startActive;

}

void SceneTrapecios::createPlatforms(physx::PxVec3 pos)
{
    // SUELO
    PxRigidStatic* _suelo = gPhysics->createRigidStatic(PxTransform(pos));

    // Crear material del suelo: poca fricción, muy elástico
    //physx::PxMaterial* sueloMat = gPhysics->createMaterial(0.0f, 0.0f, 0.9f); // friction: 0, restitution: 0.9

    // Crear forma del suelo y asignarle el material
    physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(5, 0.5, 2));

    PxMaterial* sueloMat = gPhysics->createMaterial(
        0.5f,  // fricción estática
        0.5f,  // fricción dinámica
        0.0f   // restitución = 0 → sin rebote
    );


    shapeSuelo->setMaterials(&sueloMat, 1);

    //physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(100, 0.1, 100));
    _suelo->attachShape(*shapeSuelo);
    _gScene->addActor(*_suelo);

    // Pintar suelo
    RenderItem* item;
    item = new RenderItem(shapeSuelo, _suelo, { 0.8, 0.8,0.8,1 });

    _statics.push_back(_suelo);
}

void SceneTrapecios::createPlayer(float masa)
{
    // Posición inicial del jugador en el suelo
    PxVec3 playerPos(62, 40.5, 35);

    // Crear actor dinámico
    PxRigidDynamic* player = gPhysics->createRigidDynamic(PxTransform(playerPos));

    // Crear forma (caja o esfera)
    physx::PxShape* playerShape = CreateShape(PxBoxGeometry(1.0f, 2.0f, 1.0f));
    player->attachShape(*playerShape);

    // Masa y centro de masa
    PxRigidBodyExt::updateMassAndInertia(*player, masa);

    // Añadir al escenario
    _gScene->addActor(*player);


    // Crear render item para que se vea
    RenderItem* rPlayer = new RenderItem(playerShape, player, { 0.8f, 0.2f, 0.2f, 1.0f });
    RegisterRenderItem(rPlayer);

    // Guardar en vector de render o miembro de la clase si lo necesitas
    _scenary.push_back(rPlayer);

    // Guardar referencia global para controlar
    _player = player;

    // Bloqueamos la rotación en los tres ejes (X, Y, Z)
    _player->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
    _player->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
    _player->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);

    _rigids.push_back(player);
}

void SceneTrapecios::createMalla()
{
    // SUELO
    PxRigidStatic* _suelo = gPhysics->createRigidStatic(PxTransform({ 35, 17, 35 }));

    // Crear forma del suelo y asignarle el material
    physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(38, 0.5, 10));

    PxMaterial* sueloMat = gPhysics->createMaterial(
        0.2f,  // fricción estática
        0.2f,  // fricción dinámica
        0.8f   // restitución = 0 → sin rebote
    );

    shapeSuelo->setMaterials(&sueloMat, 1);

    _suelo->attachShape(*shapeSuelo);
    _gScene->addActor(*_suelo);

    // Pintar suelo
    RenderItem* item;
    item = new RenderItem(shapeSuelo, _suelo, { 0.8, 0.8,0.8,1 });

    _statics.push_back(_suelo);
}

void SceneTrapecios::recogerParticula()
{
    _hasCollectedParticle = true;
}

void SceneTrapecios::checkPlayerCollectible()
{
    if (!_player || !_springSys->getMovingPart() || _hasCollectedParticle)
        return;

    // Obtener posiciones
    PxVec3 playerPos = _player->getGlobalPose().p;
    PxVec3 particlePos = _springSys->getMovingPart()->getTr()->p; // o getPos()

    // Definir radio de colisión
    float playerRadius = 3.0f;
    float particleRadius = 2.0f;

    float distSq = (playerPos - particlePos).magnitudeSquared();

    if (distSq <= (playerRadius + particleRadius) * (playerRadius + particleRadius))
    {
        // Colisión detectada
        recogerParticula();
    }
}

void SceneTrapecios::handleContact(PxRigidActor* a, PxRigidActor* b)
{
    // Ver es el player
    bool aIsPlayer = (a == _player);
    bool bIsPlayer = (b == _player);

    // si no lo es... salir
    if (!aIsPlayer && !bIsPlayer) return;

    // Comprobar cual es el otro
    PxRigidActor* other = nullptr;
    if (aIsPlayer) {
        other = b;
    }
    else {
        other = a;
    }

    // --- SI OTHER ES SUELO... ---
    // Resetear el estado de salto
    if (other->getType() == PxActorType::eRIGID_STATIC)
    {
        _hasJumped = false;
    }

    // --- SI ES UN TRAPECIO... ---
    // Attach con trapecio
    if (_playerJoint == nullptr && !_pendingAttachRegistered) // solo si se puede
    {
        for (size_t i = 0; i < _trapecios.size(); ++i)
        {
            if (other == _trapecios[i].palo2)
            {
                _pendingAttachPalo = (PxRigidDynamic*)_trapecios[i].palo2;
                _pendingAttachRegistered = true;

                // en trapecio se puede saltar
                _hasJumped = false;
                break;
            }
        }
    }

}

void SceneTrapecios::attachPlayerToTrapecio(PxRigidDynamic* palo)
{
    // crear joint temporal
    _playerJoint = PxDistanceJointCreate(*gPhysics,
        _player, PxTransform(PxVec3(0, 1.0f, 0)),
        palo, PxTransform(PxVec3(0, -6.0f, 0)));

    if (_playerJoint) {
        _playerJoint->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);
        _playerJoint->setMaxDistance(0.1f);
    }
}





