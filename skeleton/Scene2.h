#pragma once
#include "Scene.h"
#include <vector>

class ProyectilSystem;
class FireParticleSystem;
class FireworksParticleSystem;
class ConfettiParticleSystem;
class RenderItem;

struct FireCircle
{
	PxVec3 centro;    // posición del centro
	float radio;      // radio del círculo
	float alturaMin;  // límite inferior
	float alturaMax;  // límite superior
	bool passed;      // si ya fue atravesado
};

class Scene2 : public Scene
{
private:
	// SISTEMAS
	ProyectilSystem* _proyectilSys;
	std::vector<FireParticleSystem*> _firesInScene; //para hacer el circulo
	FireworksParticleSystem* _firework;
	std::vector<ConfettiParticleSystem*> _confettis;
	std::vector<RenderItem*> _scenary;
	std::vector<FireCircle> _fireCircles;

	bool _hasPassedFire;
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;

	bool _gravityOn = true;
	bool _windOn = true;
	bool _windWhirlOn = false;

	int puntuacion = 0;

	bool _hasShotRossa = false;
	bool _hasShotCaleb = false;
	bool _hasShotTim = false;
	int _who_has_shot = -1;

	bool changeScene = false;

	const float RESET_DELAY = 3.0f; // 3 segundos
	bool _isWin = false;
	float _winTimer = 0.0f;

public:
	Scene2(PxPhysics* physics);
	~Scene2();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;

	bool handleKey(unsigned char key, const PxTransform& camera);

	void startCelebration();
	void createNewFirework();
	void createWalls();
	void createFireCircles();

	SceneType getType() override { return GAME; }
	bool getChangeScene() { return changeScene; }
};

