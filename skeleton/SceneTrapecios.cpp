#include "SceneTrapecios.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "MyContactCallback.h"
#include <iostream>

#include "CollectibleParticleSystem.h"
#include "ExplosionRigidBodySystem.h"
#include "FireParticleSystem.h"

#include <PxPhysicsAPI.h>

extern bool _trapeciosText;
extern bool _victory;
extern bool _lose;

SceneTrapecios::SceneTrapecios(PxPhysics* physics, PxScene* scene) : Scene(physics)
{
    set_gScene(scene);
}

SceneTrapecios::~SceneTrapecios()
{
}

void SceneTrapecios::init()
{
    _trapeciosText = true;


	// CALLBACK
    _myCallback = new MyContactCallback(this);
    
    // ---- DECORACION ----
	createDeco();
    createFires(physx::PxVec3(-10, 55, 35));

    // ---- CAMERA ----
    PxVec3 centro(35, 40, 35);
    Camera* cam = GetCamera();
    _initPosCamera = cam->getEye();
    _initDirCamera = cam->getDir();
    cam->setTransform(PxVec3(centro.x, centro.y - 10.0, centro.z - 45.0));
    cam->setDir(PxVec3(0.0, 0.13, 1));
    cam->setHumanCannonMode(true);

	// ---- JOINTS/TRAPECIOS ----
    createTrapecio(PxVec3(60, 53.5, 35), false);
    createTrapecio(PxVec3(20, 53.5, 35), true);

    // ---- PLATAFORMAS ----
    createPlatforms(PxVec3(75, 33.5, 35));
    _winPlatform = createPlatforms(PxVec3(-5, 33.5, 35));

	// ---- PLAYER ----
    createPlayer(60.0f);

	// ---- SUELO (CAMA ELASTICA) ----
    createMalla();

    // ---- PARTICULA RECOGIBLE ----
    //Material
    PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    _springSys = new CollectibleParticleSystem(gMaterial, initialCollectiblePos);
    _staticParticle = _springSys->getStaticPart();

    // CONFETTIS
    createConfettiSys(PxVec3(70, 60, 35));
    createConfettiSys(PxVec3(10, 60, 35));
}

void SceneTrapecios::update(double t)
{
    if (!_player || _statics.empty()) return;
    
    if (_isGameOver) {
        _gameOverTimer += t;

        if (_gameOverTimer >= RESET_DELAY) {
            _isGameOver = false;
            _gameOverTimer = 0.0f;

            // reiniciar pos player
            _player->setGlobalPose({ 72, 40.5, 35 });
            if (_player) {
                _player->setLinearVelocity(PxVec3(0, 0, 0));
                _player->setAngularVelocity(PxVec3(0, 0, 0));
            }

            //reiniciar muelle
            _springSys->setStaticPos(initialCollectiblePos);
            _hasCollectedParticle = false;

            _lose = false;
        }
        return;
    }
    
    if (!_win_game)
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

    }
    

    // Actualizar muelle
    if (_hasCollectedParticle && _staticParticle && _player)
    {
        PxVec3 playerPos = _player->getGlobalPose().p;

        PxVec3 offset(0.0f, 1.0f, 0.0f); // ejemplo
        _springSys->setStaticPos(playerPos + offset);
    }

    _springSys->update(t);
    
    // confetti
    if (_win_game)
    {
        for (auto c : _confettis)
        {
            c->update(t);
        }
    }
    
    // ---- FILA FUEGOS -----
    for (auto a : _firesInScene)
    {
        a->update(t);
    }
}

void SceneTrapecios::clear()
{
    _start_game = false;
    _win_game = false;
    _trapeciosText = false;

    // 1. Limpiar Callbacks
    if (_gScene) _gScene->setSimulationEventCallback(nullptr);
    if (_myCallback) { delete _myCallback; _myCallback = nullptr; }

    // 2. VACIADO VISUAL
    for (auto item : _scenary) {
        if (item) {
            DeregisterRenderItem(item);
            const_cast<RenderItem*>(item)->actor = nullptr;
        }
    }
    _scenary.clear(); // Vaciamos nuestra lista de rastreo

    for (auto*& a : _firesInScene)
    {
        delete a;
        a = nullptr;
    }
    _firesInScene.clear();

    // 3. ELIMINAR JOINTS (Antes que los actores)
    if (_playerJoint) { _playerJoint->release(); _playerJoint = nullptr; }
    for (auto& t : _trapecios) {
        if (t.joint) { t.joint->release(); t.joint = nullptr; }
    }

    // 4. ELIMINAR RÍGIDOS (Player y palos móviles)
    for (auto r : _rigids) {
        if (r) {
            _gScene->removeActor(*r);
            PxU32 nShapes = r->getNbShapes();
            physx::PxShape* shapes[8];
            r->getShapes(shapes, nShapes);
            for (PxU32 i = 0; i < nShapes; i++) shapes[i]->release();

            if (r->userData) {
                r->userData = nullptr;
            }
            r->release();
        }
    }
    _rigids.clear();
    _player = nullptr;

    // 5. ELIMINAR ESTÁTICOS (Malla, Suelo, Columnas)
    for (auto s : _statics) {
        if (s) {
            _gScene->removeActor(*s);
            
            PxU32 nShapes = s->getNbShapes();
            physx::PxShape* shapes[8];
            s->getShapes(shapes, nShapes);

            for (PxU32 i = 0; i < nShapes; i++) {
                shapes[i]->release();
            }
            
            s->release();
            s = nullptr;
        }
    }
    _statics.clear();

    // 6. Sistemas de partículas y cámara
    if (_springSys) { delete _springSys; _springSys = nullptr; }
    for (auto c : _confettis) delete c;
    _confettis.clear();

    Camera* cam = GetCamera();
    cam->setTransform(_initPosCamera);
    cam->setDir(_initDirCamera);
    cam->setHumanCannonMode(false);
    _victory = false;
}

bool SceneTrapecios::handleKey(unsigned char key, const PxTransform& camera)
{
    switch (key)
    {
        case 't':
        {
            startGame();

            break;
        }
        case ' ': // Salto
        {
            if (_start_game)
            {
                if (_playerJoint)
                {
                    // liberar
                    _playerJoint->release();
                    _playerJoint = nullptr;

                    // despertar al actor
                    _player->wakeUp();

                    // aplicar el impulso
                    PxVec3 impulsoSalto(-5500.0f, 5500.0f, 0.0f);
                    _player->addForce(impulsoSalto, PxForceMode::eIMPULSE);

                    _hasJumped = true;
                }
                else if (!_hasJumped) // Salto normal desde el suelo
                {
                    PxVec3 impulsoSuelo(-6000.0f, 10200.0f, 0.0f);
                    _player->addForce(impulsoSuelo, PxForceMode::eIMPULSE);
                    _hasJumped = true;
                }
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
    PxMaterial* gMaterial = gPhysics->createMaterial(0.0f, 0.3f, 0.0f);

    // --- SUELO ---
    PxBoxGeometry gBox = PxBoxGeometry(60.0f, 1.0f, 50.0f);
    PxTransform boxPose(PxVec3(35, 10, 30));
    // Creamos el actor estático
    PxRigidStatic* groundActor = gPhysics->createRigidStatic(boxPose);
    physx::PxShape* squareShape = gPhysics->createShape(gBox, *gMaterial);
    groundActor->attachShape(*squareShape);
    _gScene->addActor(*groundActor); // Añadir a la escena física
    _statics.push_back(groundActor);

    // Parte visual
    RenderItem* rBox = new RenderItem(squareShape, groundActor, Vector4(0.8f, 0.30f, 0.11f, 1.0f));
    _scenary.push_back(rBox);


    // --- PALO IZQUIERDO ---
    PxBoxGeometry gPaloIzq = PxBoxGeometry(1.0f, 36.0f, 2.0f);
    PxTransform paloPoseIzq(PxVec3(-10, 20, 35));
    PxRigidStatic* paloIzqActor = gPhysics->createRigidStatic(paloPoseIzq);
    physx::PxShape* paloShapeIzq = gPhysics->createShape(gPaloIzq, *gMaterial);
    paloIzqActor->attachShape(*paloShapeIzq);
    _gScene->addActor(*paloIzqActor);
    _statics.push_back(paloIzqActor);

    RenderItem* rPaloIzq = new RenderItem(paloShapeIzq, paloIzqActor, Vector4(0.8f, 0.30f, 0.11f, 1.0f));
    _scenary.push_back(rPaloIzq);

    // --- PALO DERECHO ---
    PxBoxGeometry gPaloDer = PxBoxGeometry(1.4f, 36.0f, 2.0f);
    PxTransform paloPoseDer(PxVec3(80, 20, 35));
    PxRigidStatic* paloDerActor = gPhysics->createRigidStatic(paloPoseDer);
    physx::PxShape* paloShapeDer = gPhysics->createShape(gPaloDer, *gMaterial);
    paloDerActor->attachShape(*paloShapeDer);
    _gScene->addActor(*paloDerActor);
    _statics.push_back(paloDerActor);

    RenderItem* rPaloDer = new RenderItem(paloShapeDer, paloDerActor, Vector4(0.8f, 0.30f, 0.11f, 1.0f));
    _scenary.push_back(rPaloDer);

    // --- PALO SUPERIOR ---
    PxBoxGeometry gPaloSup = PxBoxGeometry(45.0f, 0.4f, 2.0f);
    PxTransform paloPoseSup(PxVec3(35, 54, 35));
    PxRigidStatic* paloSupActor = gPhysics->createRigidStatic(paloPoseSup);
    physx::PxShape* paloShapeSup = gPhysics->createShape(gPaloSup, *gMaterial);
    paloSupActor->attachShape(*paloShapeSup);
    _gScene->addActor(*paloSupActor);
    _statics.push_back(paloSupActor);

    RenderItem* rPaloSup = new RenderItem(paloShapeSup, paloSupActor, Vector4(0.8f, 0.30f, 0.11f, 1.0f));
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

    RenderItem* item = new RenderItem(shape, palo1, Vector4(0.94f, 0.5f, 0.04f, 1.0f));
    _gScene->addActor(*palo1);
    _scenary.push_back(item);
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


    RenderItem* item2 = new RenderItem(shape2, palo2, Vector4(0.8f, 0.07f, 0.07f, 1.0f));
    _gScene->addActor(*palo2);
    _scenary.push_back(item2);
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

physx::PxRigidStatic* SceneTrapecios::createPlatforms(physx::PxVec3 pos)
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
    item = new RenderItem(shapeSuelo, _suelo, Vector4(0.8f, 0.07f, 0.07f, 1.0f));

    _statics.push_back(_suelo);
    _scenary.push_back(item);

    return _suelo;
}

void SceneTrapecios::createPlayer(float masa)
{
    // Posición inicial del jugador en el suelo
    PxVec3 playerPos(72, 40.5, 35);

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
    RenderItem* rPlayer = new RenderItem(playerShape, player, Vector4(0.94f, 0.5f, 0.04f, 1.0f));
  
    // Guardar en vector de render o miembro de la clase si lo necesitas
    _scenary.push_back(rPlayer);

    // Guardar referencia global para controlar
    _player = player;


    // Bloquea todas las rotaciones para que no se caiga ni gire
    _player->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
    _player->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
    _player->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);

    _rigids.push_back(player);
}

void SceneTrapecios::createMalla()
{
    // SUELO
    mallaActor = gPhysics->createRigidStatic(PxTransform({ 35, 17, 35 }));

    PxMaterial* sueloMat = gPhysics->createMaterial(
        0.2f,  // fricción estática
        0.2f,  // fricción dinámica
        0.8f   // restitución = 0 → sin rebote
    );
    // Crear forma del suelo y asignarle el material
    physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(45, 0.5, 10));
    shapeSuelo->setMaterials(&sueloMat, 1);

    mallaActor->attachShape(*shapeSuelo);
    _gScene->addActor(*mallaActor);

    // Pintar suelo
    RenderItem* item;
    item = new RenderItem(shapeSuelo, mallaActor, Vector4(1.0f, 0.6f, 0.3f, 1.0f));
   
    _scenary.push_back(item);
    
    _statics.push_back(mallaActor);
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

    
    // --- SI TOCA LA MALLA ---
    // (Asegúrate de tener guardado el puntero de la malla en _mallaActor al crearla)
    if (other == mallaActor)
    {
        loseGame();
        return;
    }

    // --- SI ESTÁ POR DEBAJO DE LA MALLA ---
    // También es bueno comprobar la altura por si el jugador cae fuera de los límites físicos
    float playerY = _player->getGlobalPose().p.y;
    float mallaY = mallaActor->getGlobalPose().p.y;

    if (playerY < (mallaY - 1.0f)) // Si cae 1 metro por debajo del nivel de la malla
    {
        loseGame();
        return;
    }

    // --- SI OTHER ES WINNING PLATFORM ---
    if (other == _winPlatform)
    {
        // A. Comprobar que el jugador está por encima de la plataforma
        // Obtenemos la altura del jugador y de la plataforma
        float playerY = _player->getGlobalPose().p.y;
        float platformY = _winPlatform->getGlobalPose().p.y;

        // B. Comprobar que la velocidad es descendente (está aterrizando)
        float velY = _player->getLinearVelocity().y;

        // Si la posición del player es mayor a la plataforma + un pequeño margen
        // y el jugador no está subiendo como un cohete
        if ((playerY > (platformY + 0.5f) && velY <= 0.1f) && _hasCollectedParticle)
        {
            winGame();
            return;
        }
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
    _playerJoint = PxDistanceJointCreate(*gPhysics,
        _player, PxTransform(PxVec3(0, 1.0f, 0)),
        palo, PxTransform(PxVec3(0, -5.5f, 0)));

    if (_playerJoint) {
        // Evita que el player choque con el palo mientras están unidos
        _playerJoint->setConstraintFlag(PxConstraintFlag::eCOLLISION_ENABLED, false);

        _playerJoint->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);
        _playerJoint->setMaxDistance(0.1f);
    }
}

void SceneTrapecios::createConfettiSys(physx::PxVec3 pos)
{
    // ---- SISTEMA DE RIGIDOS ----
    // Anadir un actor dinamico
    PxRigidDynamic* new_solid;
    new_solid = gPhysics->createRigidDynamic(PxTransform({ 50,200,-80 }));
    new_solid->setLinearVelocity({ 0,5,0 });
    new_solid->setAngularVelocity({ 0,0,0 });
    physx::PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
    new_solid->attachShape(*shape_ad);
    ExplosionRigidBodySystem* _expSys = new ExplosionRigidBodySystem(new_solid, gPhysics, _gScene);

    _expSys->setSystemPosition(pos);
    _expSys->setActive(false);

    _confettis.push_back(_expSys);
}

void SceneTrapecios::createFires(physx::PxVec3 pos)
{
    PxVec3 inicioFilaDer(pos.x + 0.0f, pos.y, pos.z);
    int numFuegosFila = 7;
    float espaciado = 15.0f; // Distancia entre cada fuego

    int mid = numFuegosFila / 2;

    for (int i = 0; i < numFuegosFila; ++i)
    {
        // Calculamos la X sumando el espaciado multiplicado por el índice
        // Esto los alinea: fuego0 en X, fuego1 en X+2, fuego2 en X+4...
        float x = inicioFilaDer.x + (i * espaciado);
        float y = inicioFilaDer.y;
        float z = inicioFilaDer.z;

        Particula* pAux = new Particula(Vector3(x, y, z), Vector3(0, 2, 0), 0.98, 0.1);

        // Color naranja más claro y saturado como pediste
        pAux->setColor(Vector4(1.0f, 0.5f, 0.1f, 1.0f));
        pAux->setTimeVida(0.3);

        FireParticleSystem* _firePartSystem = new FireParticleSystem(pAux, gPhysics);
        _firePartSystem->setActiveWhirlWind(false);
        _firePartSystem->setActiveWind(true);
        _firesInScene.push_back(_firePartSystem);

        if (i == mid) {
            _middleFire = _firePartSystem;
        }
    }
}

void SceneTrapecios::startGame()
{
    // empezar a mover primer trapecio
    if (!_trapecios.empty())
    {
        _trapecios[0].active = true;                // activa la lógica del motor
        _trapecios[0].motorVel = 1.0f;              // asegura velocidad inicial positiva
        _trapecios[0].palo2->wakeUp();              // despierta el actor dinámico
        _trapecios[0].joint->setDriveVelocity(_trapecios[0].motorVel); // aplica velocidad al motor

    }

    // habilitar salto
    _start_game = true;
}

void SceneTrapecios::winGame()
{
    std::cout << "ganaste" << '\n';
    _win_game = true;
    _victory = true;

    // Detener al jugador para que no siga cayendo
    if (_player) {
        _player->setLinearVelocity(PxVec3(0, 0, 0));
        _player->setAngularVelocity(PxVec3(0, 0, 0));
    }

    // activar confetti
    for (auto c : _confettis)
    {
        c->setActive(true);
    }

    // activar remolino
    _middleFire->setActiveWhirlWind(true);
}

void SceneTrapecios::loseGame()
{
    if (!_isGameOver) {
        std::cout << "PERDISTE. Reiniciando en " << RESET_DELAY << " segundos..." << std::endl;
        _isGameOver = true;
        _gameOverTimer = 0.0f;
        _lose = true;
    }
}





