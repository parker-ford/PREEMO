#ifndef __ROOT__
#define __ROOT__

#include <iostream>
#include <cassert>
#include <vector>

#include "PreemoRenderingManager.h"

namespace preemo {

	class Root {
	public:
		bool StartUp(void* window);
		void ShutDown();
		bool IsRunning();
		void Run();
	};

}
#endif // !__ROOT__
