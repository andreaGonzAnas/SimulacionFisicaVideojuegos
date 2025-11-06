#pragma once
#include "Scene.h"

class SceneManager
{
private:
	Scene* _currentScene;

public:
	SceneManager() : _currentScene(nullptr) {}
	~SceneManager() { clearCurrentScene(); }

    Scene* getCurrentScene() { return _currentScene; }

    //Setear escena actual
    void setScene(Scene* newScene)
    {
        clearCurrentScene();
        _currentScene = newScene;
        if (_currentScene)
            _currentScene->init();
    }

    void update(double t)
    {
        if (_currentScene)
            _currentScene->update(t);
    }

    void clearCurrentScene()
    {
        if (_currentScene) {
            _currentScene->clear();
            delete _currentScene;
            _currentScene = nullptr;
        }
    }
};

