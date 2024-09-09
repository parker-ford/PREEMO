#include "PreemoRenderingManager.h"

namespace preemo {
	RenderingManager::Device::Device() = default;

	RenderingManager::Device::Device(Adapter adapter, wgpu::DeviceDescriptor const* descriptor)
	{
		wgpuDevice = RequestDeviceSynchronous(adapter.wgpuAdapter, descriptor);

		// A function that is invoked whenever there is an error in the use of the device
		auto onDeviceError = [](WGPUErrorType type, char const* message, void* /* pUserData */) {
			std::cout << "Uncaptured device error: type " << type;
			if (message) std::cout << " (" << message << ")";
			std::cout << std::endl;
			};
		wgpuDeviceSetUncapturedErrorCallback(wgpuDevice, onDeviceError, nullptr /* pUserData */);
	}

	wgpu::Device RenderingManager::Device::RequestDeviceSynchronous(wgpu::Adapter adapter, wgpu::DeviceDescriptor const* descriptor) {
		struct UserData {
			wgpu::Device device = nullptr;
			bool requestEnded = false;
		};
		UserData userData;

		auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* pUserData) {
			UserData& userData = *reinterpret_cast<UserData*>(pUserData);
			if (status == WGPURequestDeviceStatus_Success) {
				userData.device = device;
			}
			else {
				std::cout << "Could not get WebGPU device: " << message << std::endl;
			}
			userData.requestEnded = true;
			};

		wgpuAdapterRequestDevice(
			adapter,
			descriptor,
			onDeviceRequestEnded,
			(void*)&userData
		);

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		while (!userData.requestEnded) {
			emscripten_sleep(100);
		}
#endif // __EMSCRIPTEN__

		assert(userData.requestEnded);

		if (!userData.device) {
			std::cout << "Failed to request device" << std::endl;
			return nullptr;
		}

		return userData.device;
	}

	void RenderingManager::Device::Inspect() {
		std::vector<WGPUFeatureName> features;
		size_t featureCount = wgpuDeviceEnumerateFeatures(wgpuDevice, nullptr);
		features.resize(featureCount);
		wgpuDeviceEnumerateFeatures(wgpuDevice, features.data());

		std::cout << "Device features:" << std::endl;
		std::cout << std::hex;
		for (auto f : features) {
			std::cout << " - 0x" << f << std::endl;
		}
		std::cout << std::dec;

		WGPUSupportedLimits limits = {};
		limits.nextInChain = nullptr;

#ifdef WEBGPU_BACKEND_DAWN
		bool success = wgpuDeviceGetLimits(wgpuDevice, &limits) == WGPUStatus_Success;
#else
		bool success = wgpuDeviceGetLimits(wgpuDevice, &limits);
#endif

		if (success) {
			std::cout << "Device limits:" << std::endl;
			std::cout << " - maxTextureDimension1D: " << limits.limits.maxTextureDimension1D << std::endl;
			std::cout << " - maxTextureDimension2D: " << limits.limits.maxTextureDimension2D << std::endl;
			std::cout << " - maxTextureDimension3D: " << limits.limits.maxTextureDimension3D << std::endl;
			std::cout << " - maxTextureArrayLayers: " << limits.limits.maxTextureArrayLayers << std::endl;
			std::cout << " - maxBindGroups: " << limits.limits.maxBindGroups << std::endl;
			std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
			std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
			std::cout << " - maxSampledTexturesPerShaderStage: " << limits.limits.maxSampledTexturesPerShaderStage << std::endl;
			std::cout << " - maxSamplersPerShaderStage: " << limits.limits.maxSamplersPerShaderStage << std::endl;
			std::cout << " - maxStorageBuffersPerShaderStage: " << limits.limits.maxStorageBuffersPerShaderStage << std::endl;
			std::cout << " - maxStorageTexturesPerShaderStage: " << limits.limits.maxStorageTexturesPerShaderStage << std::endl;
			std::cout << " - maxUniformBuffersPerShaderStage: " << limits.limits.maxUniformBuffersPerShaderStage << std::endl;
			std::cout << " - maxUniformBufferBindingSize: " << limits.limits.maxUniformBufferBindingSize << std::endl;
			std::cout << " - maxStorageBufferBindingSize: " << limits.limits.maxStorageBufferBindingSize << std::endl;
			std::cout << " - minUniformBufferOffsetAlignment: " << limits.limits.minUniformBufferOffsetAlignment << std::endl;
			std::cout << " - minStorageBufferOffsetAlignment: " << limits.limits.minStorageBufferOffsetAlignment << std::endl;
			std::cout << " - maxVertexBuffers: " << limits.limits.maxVertexBuffers << std::endl;
			std::cout << " - maxVertexAttributes: " << limits.limits.maxVertexAttributes << std::endl;
			std::cout << " - maxVertexBufferArrayStride: " << limits.limits.maxVertexBufferArrayStride << std::endl;
			std::cout << " - maxInterStageShaderComponents: " << limits.limits.maxInterStageShaderComponents << std::endl;
			std::cout << " - maxComputeWorkgroupStorageSize: " << limits.limits.maxComputeWorkgroupStorageSize << std::endl;
			std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
			std::cout << " - maxComputeWorkgroupSizeX: " << limits.limits.maxComputeWorkgroupSizeX << std::endl;
			std::cout << " - maxComputeWorkgroupSizeY: " << limits.limits.maxComputeWorkgroupSizeY << std::endl;
			std::cout << " - maxComputeWorkgroupSizeZ: " << limits.limits.maxComputeWorkgroupSizeZ << std::endl;
			std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.limits.maxComputeWorkgroupsPerDimension << std::endl;
		}
	}
}