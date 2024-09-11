#include "PreemoRenderPipeline.h"
#include "PreemoRenderingManager.h"

namespace preemo {

	extern RenderingManager* g_RenderingManager;

	// We embbed the source of the shader module here
	const char* shaderSource = R"(
	/**
	 * A structure with fields labeled with vertex attribute locations can be used
	 * as input to the entry point of a shader.
	 */
	struct VertexInput {
		@location(0) position: vec2f,
		@location(1) color: vec3f,
	};

	/**
	 * A structure with fields labeled with builtins and locations can also be used
	 * as *output* of the vertex shader, which is also the input of the fragment
	 * shader.
	 */
	struct VertexOutput {
		@builtin(position) position: vec4f,
		// The location here does not refer to a vertex attribute, it just means
		// that this field must be handled by the rasterizer.
		// (It can also refer to another field of another struct that would be used
		// as input to the fragment shader.)
		@location(0) color: vec3f,
	};

	@vertex
	fn vs_main(in: VertexInput) -> VertexOutput {
		//                         ^^^^^^^^^^^^ We return a custom struct
		var out: VertexOutput; // create the output struct
		out.position = vec4f(in.position, 0.0, 1.0); // same as what we used to directly return
		out.color = in.color; // forward the color attribute to the fragment shader
		return out;
	}

	@fragment
	fn fs_main(in: VertexOutput) -> @location(0) vec4f {
		//     ^^^^^^^^^^^^^^^^ Use for instance the same struct as what the vertex outputs
		return vec4f(in.color, 1.0); // use the interpolated color coming from the vertex shader
	}
	)";

	RenderPipeline::RenderPipeline() {
		// Load the shader module
		wgpu::ShaderModuleDescriptor shaderDesc;
#ifdef WEBGPU_BACKEND_WGPU
		shaderDesc.hintCount = 0;
		shaderDesc.hints = nullptr;
#endif
		std::cout << "Global Rendering Manager Pointer: " << g_RenderingManager << std::endl;


		// We use the extension mechanism to specify the WGSL part of the shader module descriptor
		wgpu::ShaderModuleWGSLDescriptor shaderCodeDesc;
		// Set the chained struct's header
		shaderCodeDesc.chain.next = nullptr;
		shaderCodeDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
		// Connect the chain
		shaderDesc.nextInChain = &shaderCodeDesc.chain;
		shaderCodeDesc.code = shaderSource;
		wgpu::ShaderModule shaderModule = g_RenderingManager->getWGPUDevice().createShaderModule(shaderDesc);

		//// Create the render pipeline
		wgpu::RenderPipelineDescriptor pipelineDesc;


		//Configure vertex pipeline
		wgpu::VertexBufferLayout vertexBufferLayout;
		std::vector<wgpu::VertexAttribute> vertexAttribs(2);

		vertexAttribs[0].shaderLocation = 0; //@location(0)
		vertexAttribs[0].format = wgpu::VertexFormat::Float32x2;
		vertexAttribs[0].offset = 0;

		vertexAttribs[1].shaderLocation = 1; //@location(1)
		vertexAttribs[1].format = wgpu::VertexFormat::Float32x3;
		vertexAttribs[1].offset = 2 * sizeof(float);

		vertexBufferLayout.attributeCount = static_cast<uint32_t>(vertexAttribs.size());
		vertexBufferLayout.attributes = vertexAttribs.data();

		vertexBufferLayout.arrayStride = 5 * sizeof(float);
		vertexBufferLayout.stepMode = wgpu::VertexStepMode::Vertex;

		//// We do not use any vertex buffer for this first simplistic example
		pipelineDesc.vertex.bufferCount = 1;
		pipelineDesc.vertex.buffers = &vertexBufferLayout;

		//// NB: We define the 'shaderModule' in the second part of this chapter.
		//// Here we tell that the programmable vertex shader stage is described
		//// by the function called 'vs_main' in that module.
		pipelineDesc.vertex.module = shaderModule;
		pipelineDesc.vertex.entryPoint = "vs_main";
		pipelineDesc.vertex.constantCount = 0;
		pipelineDesc.vertex.constants = nullptr;

		//// Each sequence of 3 vertices is considered as a triangle
		pipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;

		//// We'll see later how to specify the order in which vertices should be
		//// connected. When not specified, vertices are considered sequentially.
		pipelineDesc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;

		//// The face orientation is defined by assuming that when looking
		//// from the front of the face, its corner vertices are enumerated
		//// in the counter-clockwise (CCW) order.
		pipelineDesc.primitive.frontFace = wgpu::FrontFace::CCW;

		//// But the face orientation does not matter much because we do not
		//// cull (i.e. "hide") the faces pointing away from us (which is often
		//// used for optimization).
		pipelineDesc.primitive.cullMode = wgpu::CullMode::None;

		//// We tell that the programmable fragment shader stage is described
		//// by the function called 'fs_main' in the shader module.
		wgpu::FragmentState fragmentState;
		fragmentState.module = shaderModule;
		fragmentState.entryPoint = "fs_main";
		fragmentState.constantCount = 0;
		fragmentState.constants = nullptr;

		wgpu::BlendState blendState;
		blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
		blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
		blendState.color.operation = wgpu::BlendOperation::Add;
		blendState.alpha.srcFactor = wgpu::BlendFactor::Zero;
		blendState.alpha.dstFactor = wgpu::BlendFactor::One;
		blendState.alpha.operation = wgpu::BlendOperation::Add;

		wgpu::ColorTargetState colorTarget;
		colorTarget.format = g_RenderingManager->getSurface().format;
		colorTarget.blend = &blendState;
		colorTarget.writeMask = wgpu::ColorWriteMask::All; // We could write to only some of the color channels.

		//// We have only one target because our render pass has only one output color
		//// attachment.
		fragmentState.targetCount = 1;
		fragmentState.targets = &colorTarget;
		pipelineDesc.fragment = &fragmentState;

		//// We do not use stencil/depth testing for now
		pipelineDesc.depthStencil = nullptr;

		//// Samples per pixel
		pipelineDesc.multisample.count = 1;

		//// Default value for the mask, meaning "all bits on"
		pipelineDesc.multisample.mask = ~0u;

		//// Default value as well (irrelevant for count = 1 anyways)
		pipelineDesc.multisample.alphaToCoverageEnabled = false;
		pipelineDesc.layout = nullptr;

		wgpuRenderPipeline = g_RenderingManager->getWGPUDevice().createRenderPipeline(pipelineDesc);

		//// We no longer need to access the shader module
		shaderModule.release();
	}
	void RenderPipeline::Release()
	{
		//wgpuRenderPipeline.release();
	}
	wgpu::RenderPipeline RenderPipeline::getWGPURenderPipeline()
	{
		return wgpuRenderPipeline;
	}
}