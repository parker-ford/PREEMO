#include "PreemoSceneManager.h"
#include "Components.h"


namespace preemo {
	SceneManager* g_SceneManager = nullptr;

	bool SceneManager::StartUp()
	{
		assert(!g_SceneManager);
		g_SceneManager = new SceneManager();

		return true;
	}

	void SceneManager::ShutDown() {
		delete g_SceneManager;
	}

	SceneManager& SceneManager::GetInstance()
	{
		return *g_SceneManager;
	}
	void SceneManager::SetActiveScene(Scene* activeScene)
	{
		m_ActiveScene = activeScene;
	}
	void SceneManager::MainLoop()
	{
		m_ActiveScene->UpdateScriptableEntities();
	}

	SceneManager::SceneManager() {
		m_ComponentRegistry.RegisterComponent<TransformComponent>("TransformComponent");
	}
	SceneManager::~SceneManager() {

	}
}