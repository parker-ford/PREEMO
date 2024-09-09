#include "PreemoRenderingManager.h"

namespace preemo {
	RenderingManager::Surface::Surface() = default;

	RenderingManager::Surface::Surface(wgpu::Instance instance, void* windowPtr) {
		window = reinterpret_cast<GLFWwindow*>(windowPtr);
		wgpuSurface = glfwGetWGPUSurface(instance, window);
	}

	bool RenderingManager::Surface::ShouldClose() {
		return glfwWindowShouldClose(window);
	}

	void RenderingManager::Surface::Configure(wgpu::SurfaceConfiguration* config) {
		wgpuSurfaceConfigure(wgpuSurface, config);
	}

	wgpu::TextureView RenderingManager::Surface::GetNextSurfaceTextureView() {
		WGPUSurfaceTexture surfaceTexture;
		wgpuSurfaceGetCurrentTexture(wgpuSurface, &surfaceTexture);
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