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
		m_surface = Surface(instance.wgpu_instance, windowPtr);

		// Request Adapter
		wgpu::RequestAdapterOptions adapterOpts = {};
		Adapter adapter(instance.wgpu_instance, &adapterOpts);
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

		wgpu::RequiredLimits requiredLimits = adapter.GetRequiredLimits();
		deviceDesc.requiredLimits = &requiredLimits;
		m_device = Device(adapter, &deviceDesc);
		//device.Inspect();

		// Initialize Queue
		m_queue = wgpuDeviceGetQueue(m_device.wgpu_device);

		// Configure Surface
		wgpu::SurfaceConfiguration config = {};
		config.nextInChain = nullptr;
		config.width = 640;
		config.height = 480;
		config.usage = wgpu::TextureUsage::RenderAttachment;
		config.format = m_surface.wgpu_surface.getPreferredFormat(adapter.wgpu_adapter);
		config.viewFormatCount = 0;
		config.viewFormats = nullptr;
		config.device = m_device.wgpu_device;
		config.presentMode = wgpu::PresentMode::Fifo;
		config.alphaMode = wgpu::CompositeAlphaMode::Auto;
		m_surface.Configure(&config);

		//Release Adapter & Instance
		adapter.wgpu_adapter.release();
		instance.wgpu_instance.release();
	}

	RenderingManager::~RenderingManager() {
		//pipeline.Release();
		//PREEMO_TODO: Make class level release/unconfigure methods
		m_surface.wgpu_surface.unconfigure();
		m_queue.release();
		m_surface.wgpu_surface.release();
		m_device.wgpu_device.release();
	}

	RenderingManager& RenderingManager::GetInstance()
	{
		return *g_RenderingManager;
	}

	void RenderingManager::MainLoop()
	{
		glfwPollEvents();

		WGPUTextureView targetView = m_surface.GetNextSurfaceTextureView();
		if (!targetView) {
			std::cout << "target View null" << std::endl;
			return;
		}

		// Create a command encoder for the draw call
		WGPUCommandEncoderDescriptor encoderDesc = {};
		encoderDesc.nextInChain = nullptr;
		encoderDesc.label = "My command encoder";
		WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(m_device.wgpu_device, &encoderDesc);

		// Create the render pass that clears the screen with our color
		WGPURenderPassDescriptor renderPassDesc = {};

		// The attachment part of the render pass descriptor describes the target texture of the pass
		WGPURenderPassColorAttachment renderPassColorAttachment = {};
		renderPassColorAttachment.view = targetView;
		renderPassColorAttachment.resolveTarget = nullptr;
		renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
		renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
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

		renderPass.setPipeline(m_pipeline->getWGPURenderPipeline());

		renderPass.setVertexBuffer(0, pointBuffer, 0, pointBuffer.getSize());

		renderPass.setIndexBuffer(indexBuffer, wgpu::IndexFormat::Uint16, 0, indexBuffer.getSize());


		renderPass.drawIndexed(indexCount, 1, 0, 0, 0);

		renderPass.end();
		renderPass.release();

		// Finally encode and submit the render pass
		WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
		cmdBufferDescriptor.nextInChain = nullptr;
		cmdBufferDescriptor.label = "Command buffer";
		WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);
		wgpuCommandEncoderRelease(encoder);

		//std::cout << "Submitting command..." << std::endl;
		wgpuQueueSubmit(m_queue, 1, &command);
		wgpuCommandBufferRelease(command);
		//std::cout << "Command submitted." << std::endl;

		// At the end of the frame
		wgpuTextureViewRelease(targetView);
#ifndef __EMSCRIPTEN__
		wgpuSurfacePresent(m_surface.wgpu_surface);
#endif

		// Also move here the tick/poll but NOT the emscripten sleep
#if defined(WEBGPU_BACKEND_DAWN)
		wgpuDeviceTick(mDevice.wgpuDevice);
#elif defined(WEBGPU_BACKEND_WGPU)
		wgpuDevicePoll(m_device.wgpu_device, false, nullptr);
#elif defined(WEBGPU_BACKEND_EMSCRIPTEN)
		std::cout << "main loop" << std::endl;
#endif
	}

	bool RenderingManager::IsRunning()
	{
		return !m_surface.ShouldClose();
	}

	wgpu::Device RenderingManager::getWGPUDevice()
	{
		return m_device.wgpu_device;
	}

	RenderingManager::Surface RenderingManager::getSurface()
	{
		return m_surface;
	}

	RenderingManager::Device RenderingManager::getDevice()
	{
		return m_device;
	}

	void RenderingManager::TestPipeline()
	{
		m_pipeline = new RenderPipeline();
	}

	void RenderingManager::TestBuffers()
	{
		std::vector<float> pointData = {
			// x,   y,     r,   g,   b
			-0.5, -0.5,   1.0, 0.0, 0.0, // Point #0
			+0.5, -0.5,   0.0, 1.0, 0.0, // Point #1
			+0.5, +0.5,   0.0, 0.0, 1.0, // Point #2
			-0.5, +0.5,   1.0, 1.0, 0.0  // Point #3
		};

		std::vector<uint16_t> indexData = {
			0, 1, 2, // Triangle #0 connects points #0, #1 and #2
			0, 2, 3  // Triangle #1 connects points #0, #2 and #3
		};


		indexCount = static_cast<uint32_t>(indexData.size());

		wgpu::BufferDescriptor bufferDesc;
		bufferDesc.size = pointData.size() * sizeof(float);
		bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
		bufferDesc.mappedAtCreation = false;
		pointBuffer = m_device.wgpu_device.createBuffer(bufferDesc);

		//write geometry data to the buffer
		m_queue.writeBuffer(pointBuffer, 0, pointData.data(), bufferDesc.size);

		bufferDesc.size = indexData.size() * sizeof(uint16_t);
		bufferDesc.size = (bufferDesc.size + 3) & ~3; // round up to the next multiple of 4
		bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Index;
		indexBuffer = m_device.wgpu_device.createBuffer(bufferDesc);

		m_queue.writeBuffer(indexBuffer, 0, indexData.data(), bufferDesc.size);
	}

}