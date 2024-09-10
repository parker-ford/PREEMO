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
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		std::cout << "running.." << std::endl;
		auto callback = [](void* arg) {
			RenderingManager* pRenderingManager = reinterpret_cast<RenderingManager*>(arg);
			pRenderingManager->MainLoop();
		};
		emscripten_set_main_loop_arg(callback, &gRenderingManager, 0, true);
#else
		while (IsRunning()) {
			g_RenderingManager->MainLoop();
		}
#endif
		ShutDown();
	}
}