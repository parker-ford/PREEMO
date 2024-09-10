#include "PreemoRoot.h"
#include "PreemoRenderingManager.h"

namespace preemo {

	extern RenderingManager* g_RenderingManager;

	bool Root::StartUp(void* window)
	{
		if (!g_RenderingManager->StartUp(window)) {
			std::cout << "Failed to start up rendering manager" << std::endl;
			return false;
		}

		//PREEMO_TODO: Remove this
		g_RenderingManager->TestPipeline();


#ifdef WEBGPU_BACKEND_EMSCRIPTEN
			std::cout << "running.." << std::endl;
			auto callback = [](void* arg) {
				RenderingManager* p_RenderingManager = reinterpret_cast<RenderingManager*>(arg);
				std::cout << p_RenderingManager << std::endl;
				p_RenderingManager->MainLoop();
			};
			std::cout << "global rendering manager before callback: " << g_RenderingManager << std::endl;
			emscripten_set_main_loop_arg(callback, g_RenderingManager, 0, true);
#endif

		return true;
	}

	void Root::ShutDown()
	{
		g_RenderingManager->ShutDown();
	}

	bool Root::IsRunning()
	{
		return g_RenderingManager->IsRunning();
	}

	void Root::Run()
	{
#ifndef WEBGPU_BACKEND_EMSCRIPTEN
		while (IsRunning()) {
			g_RenderingManager->MainLoop();
		}
		ShutDown();
#endif
	}

}