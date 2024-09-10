#ifndef __PREEMO_PIPELINE__
#define __PREEMO_PIPELINE__

#include <webgpu/webgpu.hpp>
//#include "PreemoRenderingManager.h"

namespace preemo {

	//extern RenderingManager gRenderingManager;

	class RenderPipeline {
	public:
		RenderPipeline();
		void Release();
		wgpu::RenderPipeline getWGPURenderPipeline();
	private:
		wgpu::RenderPipeline wgpuRenderPipeline;
	};
}

#endif // !__PREEMO_PIPELINE__
