#pragma once

#include <webgpu/webgpu.hpp>

namespace preemo {
	class RenderPipeline {
	public:
		RenderPipeline();
		void Release();
		wgpu::RenderPipeline getWGPURenderPipeline();
	private:
		wgpu::RenderPipeline wgpuRenderPipeline;
	};
}