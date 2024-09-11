#include "PreemoRenderingManager.h"

namespace preemo {
	RenderingManager::Adapter::Adapter(wgpu::Instance instance, wgpu::RequestAdapterOptions const* options) {
		wgpu_adapter = RequestAdapterSynchronous(instance, options);
	}

	wgpu::Adapter RenderingManager::Adapter::RequestAdapterSynchronous(wgpu::Instance instance, wgpu::RequestAdapterOptions const* options) {
		struct UserData {
			wgpu::Adapter adapter = nullptr;
			bool requestEnded = false;
		};
		UserData userData;

		auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* pUserData) {
			UserData& userData = *reinterpret_cast<UserData*>(pUserData);
			if (status == wgpu::RequestAdapterStatus::Success) {
				userData.adapter = adapter;
			}
			else {
				std::cerr << "Could not get WebGPU adapter: " << message << std::endl;
			}
			userData.requestEnded = true;
		};

		wgpuInstanceRequestAdapter(
			instance, /* equivalent of navigator.gpu*/
			options,
			onAdapterRequestEnded,
			(void*)&userData
		);

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		while (!userData.requestEnded) {
			std::cout << "waiting.." << std::endl;
			emscripten_sleep(1);
		}
#endif

		assert(userData.requestEnded);

		if (!userData.adapter) {
			std::cerr << "Failed to initialize adpater" << std::endl;
			return nullptr;
		}

		return userData.adapter;
	}

	void RenderingManager::Adapter::Inspect()
	{
#ifndef WEBGPU_BACKEND_EMSCRIPTEN
		wgpu::SupportedLimits supportedLimits = {};
		supportedLimits.nextInChain = nullptr;
#ifdef WEBGPU_BACKEND_DAWN
		bool success = wgpuAdapter.getLimits(&supportedLimits) == wgpu::Status::Success;
#else
		bool success = wgpuAdapterGetLimits(wgpu_adapter, &supportedLimits);
#endif
		if (success) {
			std::cout << "Adapter limits:" << std::endl;
			std::cout << " - maxTextureDimension1D: " << supportedLimits.limits.maxTextureDimension1D << std::endl;
			std::cout << " - maxTextureDimension2D: " << supportedLimits.limits.maxTextureDimension2D << std::endl;
			std::cout << " - maxTextureDimension3D: " << supportedLimits.limits.maxTextureDimension3D << std::endl;
			std::cout << " - maxTextureArrayLayers: " << supportedLimits.limits.maxTextureArrayLayers << std::endl;
		}
#endif

		std::vector<WGPUFeatureName> features = {};

		size_t featureCount = wgpu_adapter.enumerateFeatures(nullptr);

		features.resize(featureCount);

		wgpuAdapterEnumerateFeatures(wgpu_adapter, features.data());

		std::cout << "Adapter features" << std::endl;
		std::cout << std::hex; // Write integers as hexadecimal to ease comparison with webgpu.h literals
		for (auto f : features) {
			std::cout << " - 0x" << f << std::endl;
		}
		std::cout << std::dec; // Restore decimal numbers
		wgpu::AdapterProperties properties = {};
		properties.nextInChain = nullptr;
		wgpu_adapter.getProperties(&properties);
		std::cout << "Adapter properties:" << std::endl;
		std::cout << " - vendorID: " << properties.vendorID << std::endl;
		if (properties.vendorName) {
			std::cout << " - vendorName: " << properties.vendorName << std::endl;
		}
		if (properties.architecture) {
			std::cout << " - architecture: " << properties.architecture << std::endl;
		}
		std::cout << " - deviceID: " << properties.deviceID << std::endl;
		if (properties.name) {
			std::cout << " - name: " << properties.name << std::endl;
		}
		if (properties.driverDescription) {
			std::cout << " - driverDescription: " << properties.driverDescription << std::endl;
		}
		std::cout << std::hex;
		std::cout << " - adapterType: 0x" << properties.adapterType << std::endl;
		std::cout << " - backendType: 0x" << properties.backendType << std::endl;
		std::cout << std::dec; // Restore decimal numbers

	}

	wgpu::RequiredLimits RenderingManager::Adapter::GetRequiredLimits()
	{
		wgpu::SupportedLimits supportedLimits;
		wgpu_adapter.getLimits(&supportedLimits);

		wgpu::RequiredLimits requiredLimits = wgpu::Default;

		//PREEMO_TODO: Update required limits
		requiredLimits.limits.maxVertexAttributes = 2;
		requiredLimits.limits.maxVertexBuffers = 1;
		requiredLimits.limits.maxBufferSize = 6 * 5 * sizeof(float);
		requiredLimits.limits.maxVertexBufferArrayStride = 5 * sizeof(float);

		requiredLimits.limits.maxInterStageShaderComponents = 3;
	
		requiredLimits.limits.minUniformBufferOffsetAlignment = supportedLimits.limits.minUniformBufferOffsetAlignment;
		requiredLimits.limits.minStorageBufferOffsetAlignment = supportedLimits.limits.minStorageBufferOffsetAlignment;
	
		return requiredLimits;
	}


}
