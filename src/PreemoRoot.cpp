#include "PreemoRoot.h"
#include "PreemoRenderingManager.h"
#include "PreemoSceneManager.h"

namespace preemo {

	extern RenderingManager* g_RenderingManager;
	extern SceneManager* g_SceneManager;

	bool Root::StartUp(void* window)
	{
		if (!g_RenderingManager->StartUp(window)) {
			std::cout << "Failed to start up rendering manager" << std::endl;
			return false;
		}

		if (!g_SceneManager->StartUp()) {
			std::cout << "Failed to start up scene manager" << std::endl;
			return false;
		}

		//PREEMO_TODO: Remove this
		g_RenderingManager->TestBuffers();
		g_RenderingManager->TestPipeline();

		startUpComplete = true;
		return true;
	}

	void Root::ShutDown()
	{
		g_RenderingManager->ShutDown();
		g_SceneManager->ShutDown();
	}

	bool Root::IsRunning()
	{
		return g_RenderingManager->IsRunning();
	}

	void Root::Run(Scene* activeScene)
	{
		g_SceneManager->SetActiveScene(activeScene);

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		auto callback = [](void*) {
			if (g_RenderingManager) {
				g_SceneManager->MainLoop();
				g_RenderingManager->MainLoop();
			}
		};
		emscripten_set_main_loop_arg(callback, nullptr, 0, true);
#else
		while (IsRunning()) {
			g_SceneManager->MainLoop();
			g_RenderingManager->MainLoop();
		}
		ShutDown();
#endif
	}
	
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
	
	bool Root::hasCompletedStartUp()
	{
		return startUpComplete;
	}
#endif

}