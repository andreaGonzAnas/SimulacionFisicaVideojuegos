#pragma once
#include "Scene.h"
#include <vector>

class RenderItem;
class CollectibleParticleSystem;
class Particula;
class MyContactCallback;
class ExplosionRigidBodySystem;
class FireParticleSystem;

struct Trapecio
{
	PxRigidStatic* palo1 = nullptr;
	PxRigidDynamic* palo2 = nullptr;
	PxRevoluteJoint* joint = nullptr;
	float motorVel = 0.0f;
	bool active = false;
};


class SceneTrapecios : public Scene
{
private:
	// DECO
	std::vector<RenderItem*> _scenary;
	std::vector<FireParticleSystem*> _firesInScene;
	FireParticleSystem* _middleFire;

	// CAMERA
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;

	// UPDATE
	PxRevoluteJoint* art;
	float initialMotorVel = 1.0f;

	// TRAPECIOS
	std::vector<Trapecio> _trapecios;

	// PLAYER
	PxRigidDynamic* _player;

	bool _hasJumped = false;
	bool _hasCollectedParticle = false;

	CollectibleParticleSystem* _springSys;
	Particula* _staticParticle;

	// ESTATICOS
	std::vector<PxRigidStatic*> _statics;
	std::vector<physx::PxRigidDynamic*> _rigids;


	// COLISIONES
	MyContactCallback* _myCallback = nullptr;
	physx::PxDistanceJoint* _playerJoint = nullptr;
	physx::PxRigidDynamic* _pendingAttachPalo = nullptr;
	bool _pendingAttachRegistered = false;

	// RIGIDS
	std::vector<ExplosionRigidBodySystem*> _confettis;

	// GAME
	bool _start_game = false;
	bool _win_game = false;
	physx::PxRigidStatic* _winPlatform = nullptr;
	physx::PxRigidStatic* mallaActor = nullptr;
	bool _isGameOver = false;
	float _gameOverTimer = 0.0f;
	const float RESET_DELAY = 3.0f; // 3 segundos
	PxVec3 initialCollectiblePos = { 35, 53.5, 35 };


public:


	SceneTrapecios(PxPhysics* physics, PxScene* scene);
	~SceneTrapecios();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;

	bool handleKey(unsigned char key, const PxTransform& camera);

	void createDeco();
	void createTrapecio(physx::PxVec3 pos, bool startActive = true);
	physx::PxRigidStatic* createPlatforms(physx::PxVec3 pos);
	void createPlayer(float masa);
	void createMalla();

	PxRigidDynamic* getPlayer() { return _player; }
	std::vector<Trapecio> getTrapecios() { return _trapecios; }
	physx::PxDistanceJoint* getPlayerjoint() { return _playerJoint; }

	void recogerParticula();
	void checkPlayerCollectible();

	void handleContact(PxRigidActor* a, PxRigidActor* b);
	void attachPlayerToTrapecio(PxRigidDynamic* palo);

	// CONFETTI
	void createConfettiSys(physx::PxVec3 pos);
	void createFires(physx::PxVec3 pos);

	// GAMEPLAY
	void startGame();
	void winGame();
	void loseGame();
};
