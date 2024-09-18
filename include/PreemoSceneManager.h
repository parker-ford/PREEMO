#pragma once
#include <cassert>
#include "PreemoScene.h"
#include "PreemoComponentRegistry.h"

namespace preemo {
	//class Scene;
	class SceneManager {
	public:
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		static bool StartUp();
		static void ShutDown();
		static SceneManager& GetInstance();
		void SetActiveScene(Scene* activeScene);

		void MainLoop();
	private:
		SceneManager();
		~SceneManager();
		Scene* m_ActiveScene;
	public:
		ComponentRegistry m_ComponentRegistry;
	};

}