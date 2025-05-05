#include "SceneManager.h"

bool SceneManager::AddScene(std::string _name, Scene* _scene)
{
	if (scenes.find(_name) == scenes.end())
	{
		scenes.emplace(_name, _scene);
		return true;
	}

	return false;
}

Scene* SceneManager::GetScene(std::string _name)
{
	if (scenes.find(_name) != scenes.end())
		return scenes[_name];

	return nullptr;
}

bool SceneManager::SetCurrentScene(std::string _name)
{
	if (scenes.find(_name) != scenes.end())
	{
		currentScene = scenes[_name];
		return true;
	}
	return false;
}
