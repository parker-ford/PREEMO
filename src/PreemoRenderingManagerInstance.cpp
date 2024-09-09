#include "PreemoRenderingManager.h"

namespace preemo{
	RenderingManager::Instance::Instance() {
		wgpu::InstanceDescriptor desc = {};
		desc.nextInChain = nullptr;

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		wgpuInstance = wgpuCreateInstance(nullptr);
#else
		wgpuInstance = wgpuCreateInstance(&desc);
#endif
		if (!wgpuInstance) {
			std::cerr << "Could not initialize WebGPU!" << std::endl;
		}
	}
}