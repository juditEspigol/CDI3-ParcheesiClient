#pragma once
#include "AuthenticateScene.h"
#include "RoomScene.h"

#define SCENE_MANAGER SceneManager::Instance()

class SceneManager
{
private:

    std::unordered_map<std::string, Scene*> scenes; 
    Scene* currentScene = nullptr; 

    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator =(const SceneManager&) = delete;

public:

    inline static SceneManager& Instance()
    {
        static SceneManager manager;
        return manager;
    }

    bool AddScene(std::string _name, Scene* _scene); 
    Scene* GetScene(std::string _name); 
    inline Scene* GetCurrentScene() const { return currentScene; }
    bool SetCurrentScene(std::string _name);

};