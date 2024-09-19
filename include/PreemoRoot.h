#ifndef __ROOT__
#define __ROOT__

#include <iostream>
#include <cassert>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/html5.h>
#endif // __EMSCRIPTEN__


namespace preemo {
	class Scene;
	class Root {
	public:
		bool StartUp(void* window);
		void ShutDown();
		bool IsRunning();
		void Run(Scene* scene);
		void foo();
		bool hasCompletedStartUp();
	private:
		bool startUpComplete = false;
	};

}
#endif // !__ROOT__
