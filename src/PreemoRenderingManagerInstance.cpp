#include "PreemoRenderingManager.h"

namespace preemo{
	RenderingManager::Instance::Instance() {
		wgpu::InstanceDescriptor desc = {};
		desc.nextInChain = nullptr;

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		wgpuInstance = wgpuCreateInstance(nullptr);
#else
		wgpu_instance = wgpuCreateInstance(&desc);
#endif
		if (!wgpu_instance) {
			std::cerr << "Could not initialize WebGPU!" << std::endl;
		}
	}
}