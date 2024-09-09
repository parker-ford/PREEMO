#ifndef __RENDERING_MANAGER__
#define __RENDERING_MANAGER__

#include <webgpu/webgpu.hpp>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/val.h>
#  include <emscripten/html5.h>
#endif // __EMSCRIPTEN__

#include <iostream>
#include <cassert>
#include <vector>

namespace preemo {
	class RenderingManager {
	public:
		bool StartUp(void* windowPtr);
		void ShutDown();
		void MainLoop();
		bool IsRunning();
	private:
		bool InitializeInstance(wgpu::Instance* instance);

	private:
		class Instance;
		class Adapter;
		class Device;
		class Surface;

		class Instance {
			friend class RenderingManager;
		public:
			Instance();
		private:
			wgpu::Instance wgpuInstance;
		};

		class Adapter {
			friend class RenderingManager;
			friend class RenderingManager::Device;
		public:
			Adapter(wgpu::Instance instance, wgpu::RequestAdapterOptions const* options);
			void Inspect();
		private:
			wgpu::Adapter RequestAdapterSynchronous(wgpu::Instance instance, wgpu::RequestAdapterOptions const* options);
		private:
			wgpu::Adapter wgpuAdapter;
		};

		class Device {
			friend class RenderingManager;
		public:
			Device();
			Device(Adapter adapter, wgpu::DeviceDescriptor const* options);
			void Inspect();
		private:
			wgpu::Device RequestDeviceSynchronous(wgpu::Adapter adapter, wgpu::DeviceDescriptor const* descriptor);
		private:
			wgpu::Device wgpuDevice;
		};

		class Surface {
			friend class RenderingManager;
		public:
			Surface();
			Surface(wgpu::Instance instance, void* windowPtr);
			bool ShouldClose();
			void Configure(wgpu::SurfaceConfiguration* config);
			wgpu::TextureView GetNextSurfaceTextureView();
		private:
			wgpu::Surface wgpuSurface;
			GLFWwindow* window;
		};

	private:
		wgpu::Queue queue;
		Device device;
		Surface surface;
	};
}

#endif // __RENDERING_MANAGER__
