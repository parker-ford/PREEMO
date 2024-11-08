#include "PreemoRenderingManager.h"

namespace preemo {
	RenderingManager::Surface::Surface() = default;

	RenderingManager::Surface::Surface(wgpu::Instance instance, void* windowPtr) {
		m_window = reinterpret_cast<GLFWwindow*>(windowPtr);
		wgpu_surface = glfwGetWGPUSurface(instance, m_window);
	}

	bool RenderingManager::Surface::ShouldClose() {
		return glfwWindowShouldClose(m_window);
	}

	void RenderingManager::Surface::Configure(wgpu::SurfaceConfiguration* config) {
		this->format = config->format;
		wgpuSurfaceConfigure(wgpu_surface, config);
	}

	wgpu::TextureView RenderingManager::Surface::GetNextSurfaceTextureView() {
		WGPUSurfaceTexture surfaceTexture;
		wgpuSurfaceGetCurrentTexture(wgpu_surface, &surfaceTexture);
		if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success) {
			return nullptr;
		}

		// Create a view for this surface texture
		WGPUTextureViewDescriptor viewDescriptor;
		viewDescriptor.nextInChain = nullptr;
		viewDescriptor.label = "Surface texture view";
		viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
		viewDescriptor.dimension = WGPUTextureViewDimension_2D;
		viewDescriptor.baseMipLevel = 0;
		viewDescriptor.mipLevelCount = 1;
		viewDescriptor.baseArrayLayer = 0;
		viewDescriptor.arrayLayerCount = 1;
		viewDescriptor.aspect = WGPUTextureAspect_All;
		WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

		return targetView;
	}
}