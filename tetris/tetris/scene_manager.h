#pragma once

#include "game_object.h"
#include "singleton.h"

enum class Scene
{
    kMain,
};

class _SceneManager : public Singleton<_SceneManager>
{
public:
    void moveGameObjectToScene(GameObject* go, Scene scene)
    {

    }

    void loadScene(Scene scene)
    {

    }

private:
    Scene _current_scene;
};

struct SceneManager
{
    _SceneManager* operator()()
    {
        return _SceneManager::getInstance();
    }
};