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
		//std::cout << "running.." << std::endl;
		//auto callback = [](void* arg) {
		//	//RenderingManager* p_RenderingManager = reinterpret_cast<RenderingManager*>(arg);
		//	//std::cout << "a " << p_RenderingManager << std::endl;
		//	//std::cout << "b " << g_RenderingManager << std::endl;
		//	g_RenderingManager->MainLoop();
		//	//p_RenderingManager->MainLoop();
		//};
		auto callback = [](void*) {
			g_RenderingManager->MainLoop();
		};
		std::cout << "global rendering manager before callback: " << g_RenderingManager << std::endl;
		emscripten_set_main_loop_arg(callback, nullptr, 0, true);
		//emscripten_set_main_loop_arg()
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