#include "PreemoRenderingManager.h"

namespace preemo {

	RenderingManager* g_RenderingManager = nullptr;

	bool RenderingManager::StartUp(void* windowPtr)
	{
		assert(!g_RenderingManager);
		g_RenderingManager = new RenderingManager(windowPtr);

		return true;
	}

	void RenderingManager::ShutDown()
	{
		delete g_RenderingManager;
	}

	RenderingManager::RenderingManager(void* windowPtr)
	{
		// Initialize Instance
		Instance instance = Instance();

		// Initialize Surface
		surface = Surface(instance.wgpuInstance, windowPtr);

		// Request Adapter
		wgpu::RequestAdapterOptions adapterOpts = {};
		Adapter adapter(instance.wgpuInstance, &adapterOpts);
		//adapter.Inspect();

		// Request Device
		wgpu::DeviceDescriptor deviceDesc = {};
		deviceDesc.nextInChain = nullptr;
		deviceDesc.label = "My Device";
		deviceDesc.requiredFeatureCount = 0;
		deviceDesc.requiredLimits = nullptr;
		deviceDesc.defaultQueue.nextInChain = nullptr;
		deviceDesc.defaultQueue.label = "The default queue";
#ifdef WEBGPU_BACKEND_DAWN
		wgpu::DeviceLostCallbackInfo callbackInfo;
		callbackInfo.callback = [](WGPUDevice const* device, WGPUDeviceLostReason reason, char const* message, void* /* pUserData */) {
			std::cout << "Device: " << device << std::endl;
			std::cout << "Device lost: reason " << reason;
			if (message) std::cout << " (" << message << ")";
			std::cout << std::endl;
			};
		callbackInfo.nextInChain = nullptr;
		deviceDesc.deviceLostCallbackInfo = callbackInfo;
#else
		deviceDesc.deviceLostCallback = [](WGPUDeviceLostReason reason, char const* message, void* /* pUserData */) {
			std::cout << "Device lost: reason " << reason;
			if (message) std::cout << " (" << message << ")";
			std::cout << std::endl;
			};
#endif


		mDevice = Device(adapter, &deviceDesc);
		//device.Inspect();

		// Initialize Queue
		queue = wgpuDeviceGetQueue(mDevice.wgpuDevice);

		// Configure Surface
		wgpu::SurfaceConfiguration config = {};
		config.nextInChain = nullptr;
		config.width = 640;
		config.height = 480;
		config.usage = wgpu::TextureUsage::RenderAttachment;
		config.format = surface.wgpuSurface.getPreferredFormat(adapter.wgpuAdapter);
		config.viewFormatCount = 0;
		config.viewFormats = nullptr;
		config.device = mDevice.wgpuDevice;
		config.presentMode = wgpu::PresentMode::Fifo;
		config.alphaMode = wgpu::CompositeAlphaMode::Auto;
		surface.Configure(&config);

		//Release Adapter & Instance
		adapter.wgpuAdapter.release();
		instance.wgpuInstance.release();
		
		//pipeline = new RenderPipeline();
	}

	RenderingManager::~RenderingManager() {
		//pipeline.Release();
		//PREEMO_TODO: Make class level release/unconfigure methods
		surface.wgpuSurface.unconfigure();
		queue.release();
		surface.wgpuSurface.release();
		mDevice.wgpuDevice.release();
	}

	RenderingManager& RenderingManager::GetInstance()
	{
		return *g_RenderingManager;
	}

	void RenderingManager::MainLoop()
	{
		glfwPollEvents();

		WGPUTextureView targetView = surface.GetNextSurfaceTextureView();
		if (!targetView) {
			std::cout << "target View null" << std::endl;
			return;
		}

		// Create a command encoder for the draw call
		WGPUCommandEncoderDescriptor encoderDesc = {};
		encoderDesc.nextInChain = nullptr;
		encoderDesc.label = "My command encoder";
		WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(mDevice.wgpuDevice, &encoderDesc);

		// Create the render pass that clears the screen with our color
		WGPURenderPassDescriptor renderPassDesc = {};
		renderPassDesc.nextInChain = nullptr;

		// The attachment part of the render pass descriptor describes the target texture of the pass
		WGPURenderPassColorAttachment renderPassColorAttachment = {};
		renderPassColorAttachment.view = targetView;
		renderPassColorAttachment.resolveTarget = nullptr;
		renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
		renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
		renderPassColorAttachment.clearValue = WGPUColor{ 0.9, 0.1, 0.2, 1.0 };
#ifndef WEBGPU_BACKEND_WGPU
		renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU

		renderPassDesc.colorAttachmentCount = 1;
		renderPassDesc.colorAttachments = &renderPassColorAttachment;
		renderPassDesc.depthStencilAttachment = nullptr;
		renderPassDesc.timestampWrites = nullptr;

		// Create the render pass and end it immediately (we only clear the screen but do not draw anything)
		wgpu::RenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);

		renderPass.setPipeline(pipeline->getWGPURenderPipeline());
		renderPass.draw(3, 1, 0, 0);

		renderPass.end();
		renderPass.release();

		// Finally encode and submit the render pass
		WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
		cmdBufferDescriptor.nextInChain = nullptr;
		cmdBufferDescriptor.label = "Command buffer";
		WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);
		wgpuCommandEncoderRelease(encoder);

		//std::cout << "Submitting command..." << std::endl;
		wgpuQueueSubmit(queue, 1, &command);
		wgpuCommandBufferRelease(command);
		//std::cout << "Command submitted." << std::endl;

		// At the end of the frame
		wgpuTextureViewRelease(targetView);
#ifndef __EMSCRIPTEN__
		wgpuSurfacePresent(surface.wgpuSurface);
#endif

		// Also move here the tick/poll but NOT the emscripten sleep
#if defined(WEBGPU_BACKEND_DAWN)
		wgpuDeviceTick(mDevice.wgpuDevice);
#elif defined(WEBGPU_BACKEND_WGPU)
		wgpuDevicePoll(mDevice.wgpuDevice, false, nullptr);
#elif defined(WEBGPU_BACKEND_EMSCRIPTEN)
		std::cout << "main loop" << std::endl;
#endif
	}

	bool RenderingManager::IsRunning()
	{
		return !surface.ShouldClose();
	}

	wgpu::Device RenderingManager::getWGPUDevice()
	{
		return mDevice.wgpuDevice;
	}

	RenderingManager::Surface RenderingManager::getSurface()
	{
		return surface;
	}

	RenderingManager::Device RenderingManager::getDevice()
	{
		return mDevice;
	}

	void RenderingManager::TestPipeline()
	{
		pipeline = new RenderPipeline();
	}
}