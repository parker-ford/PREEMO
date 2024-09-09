#include "PreemoRoot.h"

namespace preemo {

	RenderingManager gRenderingManager;

	bool Root::StartUp(void* window)
	{
		if (!gRenderingManager.StartUp(window)) {
			std::cout << "Failed to start up rendering manager" << std::endl;
			return false;
		}

		return true;
	}

	void Root::ShutDown()
	{
		gRenderingManager.ShutDown();
	}

	bool Root::IsRunning()
	{
		return gRenderingManager.IsRunning();
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
			gRenderingManager.MainLoop();
		}
#endif
		ShutDown();
	}
}