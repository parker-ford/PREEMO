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

		std::cout << __FILE__ << std::endl;
		std::cout << __LINE__ << std::endl;

		//PREEMO_TODO: Remove this
		g_RenderingManager->TestBuffers();
		g_RenderingManager->TestPipeline();

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
		auto callback = [](void*) {
			if (g_RenderingManager) {
				g_RenderingManager->MainLoop();
			}
		};
		emscripten_set_main_loop_arg(callback, nullptr, 0, true);
#else
		while (IsRunning()) {
			g_RenderingManager->MainLoop();
		}
		ShutDown();
#endif
	}

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
	EM_ASYNC_JS(int, do_fetch, (), {

	fetch('https://jsonplaceholder.typicode.com/todos/1')
		.then(response => response.json())
		.then(data => console.log(data))
		.catch (error => console.error('Error:', error));

		});
	void Root::foo() {
		std::cout << "foo before" << std::endl;
		do_fetch();
		std::cout << "foo after" << std::endl;
	}
#endif

}