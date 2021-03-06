
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBasicBuffering.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>

#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>

#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>
#include <Render/OcclusionTools/DepthMap.hpp>
#include <render/OcclusionTools/OcclusionOptions.hpp>
#include <Render/Pipelining/Pipelines/PipelineTools.hh>

#include "Graphic/DRBMesh.hpp"
#include "Graphic/DRBMeshData.hpp"

#include "AssetManagement\Instance\MeshInstance.hh"

#include "Render/Textures/TextureBuffer.hh"
#include "Render/ProgramResources/Types/Uniform/Sampler/SamplerBuffer.hh"

#include <Graphic/DRBCameraDrawableList.hpp>

// ca ne devrait pas etre la c'est degueulasse
#include "AssetManagement\Instance\MaterialInstance.hh"

// culling
#include <BFC/BFCBlockManagerFactory.hpp>
#include <Render\Pipelining\Prepare\MeshBufferingPrepare.hpp>
#include <Graphic/BFCCullableTypes.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Utils/Frustum.hh>

#define DEFERRED_SHADING_BUFFERING_VERTEX "deferred_shading/deferred_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_VERTEX_SKINNED "deferred_shading/deferred_shading_get_buffer_skinned.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "deferred_shading/deferred_shading_get_buffer.fp"

namespace AGE
{
	DeferredBasicBuffering *DeferredBasicBuffering::instance = nullptr;


	enum Programs
	{
		PROGRAM_BUFFERING = 0,
		PROGRAM_BUFFERING_SKINNED = 1,
		PROGRAM_NBR
	};

	DeferredBasicBuffering::DeferredBasicBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse,
		std::shared_ptr<Texture2D> normal,
		std::shared_ptr<Texture2D> specular,
		std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
		, _depth(depth)
	{
		AGE_ASSERT(depth != nullptr);

		instance = this;

		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		push_storage_output(GL_COLOR_ATTACHMENT1, normal);
		push_storage_output(GL_COLOR_ATTACHMENT2, specular);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		{
			auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_VERTEX;
			auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;

			_programs[PROGRAM_BUFFERING] = std::make_shared<Program>(Program(StringID("program_buffering", 0x32f9e8e32519519a),
			{
				std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
				std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
			}));
		}
		{
			auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_VERTEX_SKINNED;
			auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;

			_programs[PROGRAM_BUFFERING_SKINNED] = std::make_shared<Program>(Program(StringID("program_buffering_skinned", 0xfa36ccdf5a2b0029),
			{
				std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
				std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
			}));
			//_programs[PROGRAM_BUFFERING_SKINNED]->
		}
	}

	void DeferredBasicBuffering::init()
	{
		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;

		_positionBuffer = createRenderPassOutput<TextureBuffer>(_maxInstanciedShadowCaster, GL_RGBA32F, _sizeofMatrix, GL_DYNAMIC_DRAW);

	}
	void DeferredBasicBuffering::renderPass(const DRBCameraDrawableList &infos)
	{
		auto toDraw = infos.cameraMeshs;
		if (toDraw == nullptr)
			return;

		SCOPE_profile_gpu_i("DeferredBasicBuffering render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredBasicBuffering render pass");
		{
			SCOPE_profile_gpu_i("Clear buffer");
			SCOPE_profile_cpu_i("RenderTimer", "Clear buffer");

			OpenGLState::glEnable(GL_CULL_FACE);
			OpenGLState::glCullFace(GL_BACK);
			OpenGLState::glDepthMask(GL_TRUE);
			OpenGLState::glDepthFunc(GL_LESS);
			OpenGLState::glDisable(GL_BLEND);
			OpenGLState::glEnable(GL_DEPTH_TEST);
			OpenGLState::glClearColor(glm::vec4(0.f, 0.0f, 0.0f, 0.0f));
			OpenGLState::glClearStencil(1);
			OpenGLState::glEnable(GL_STENCIL_TEST);
			OpenGLState::glStencilFunc(GL_ALWAYS, 0, 0xFF);
			OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		if (infos.cameraMeshs)
		{
			auto toDraw = infos.cameraMeshs;
			SCOPE_profile_gpu_i("Draw all objects");
			SCOPE_profile_cpu_i("RenderTimer", "Draw occluded objects");

			_programs[PROGRAM_BUFFERING]->use();
			_programs[PROGRAM_BUFFERING]->get_resource<Mat4>(StringID("projection_matrix", 0x92b1e336c34a1224)).set(infos.cameraInfos.data.projection);
			_programs[PROGRAM_BUFFERING]->get_resource<Mat4>(StringID("view_matrix", 0xd15d560e7965726c)).set(infos.cameraInfos.view);
			_programs[PROGRAM_BUFFERING]->get_resource<SamplerBuffer>(StringID("model_matrix_tbo", 0x6532aea46fc01c3a)).set(_positionBuffer);
			auto matrixOffset = _programs[PROGRAM_BUFFERING]->get_resource<Vec1>(StringID("matrixOffset", 0xb870d9a9a2c195f7));

			_positionBuffer->resetOffset();

			std::shared_ptr<Painter> painter = nullptr;
			Key<Vertices> verticesKey;

			// draw for the spot light selected
			auto &generator = toDraw->getCommandOutput();
			auto &occluders = generator._commands;
			std::size_t occluderCounter = 0;

			_positionBuffer->set((void*)(generator._datas.data()), generator._datas.size() > _maxMatrixInstancied ? _maxMatrixInstancied : generator._datas.size());

			while (occluderCounter < occluders.size())
			{
				auto &current = occluders[occluderCounter];

				Key<Painter> painterKey;
				UnConcatenateKey(current.verticeKey, painterKey, verticesKey);

				if (painterKey.isValid())
				{
					_programs[PROGRAM_BUFFERING]->get_resource<Vec4>     (StringID("diffuse_color", 0x011da378d8e2a2c9)).set(current.material->diffuse);
					_programs[PROGRAM_BUFFERING]->get_resource<Sampler2D>(StringID("diffuse_map", 0x1930bc220c3b5c20)).set(current.material->diffuseTex);
					_programs[PROGRAM_BUFFERING]->get_resource<Vec4>     (StringID("specular_color", 0x747083b1ac56a160)).set(current.material->specular);
					_programs[PROGRAM_BUFFERING]->get_resource<Vec1>     (StringID("shininess_ratio", 0xf147b658a317675f)).set(current.material->shininess);
					_programs[PROGRAM_BUFFERING]->get_resource<Sampler2D>(StringID("normal_map", 0xda3297075023f6d7)).set(current.material->normalTex);
					_programs[PROGRAM_BUFFERING]->get_resource<Vec1>     (StringID("scaleUvs", 0xb70d8ad72513d8a7)).set(current.material->scaleUVs);


					painter = _painterManager->get_painter(painterKey);
					painter->instanciedDrawBegin(_programs[PROGRAM_BUFFERING]);
					matrixOffset.set(float(current.from));
					painter->instanciedDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], verticesKey, current.size);
					painter->instanciedDrawEnd();
				}
				++occluderCounter;
			}
			// Important !
			// After use, we have to recycle it ! Or
			// we will leak
			toDraw->reset();
			MeshOutput::RecycleOutput(toDraw);
		}

		if (infos.cameraSkinnedMeshs)
		{
			auto toDraw = infos.cameraSkinnedMeshs;

			if (toDraw->getCommandOutput()._commands.size() > 0)
			{

				SCOPE_profile_gpu_i("Draw all skinned objects");
				SCOPE_profile_cpu_i("RenderTimer", "Draw skinned objects");

				_programs[PROGRAM_BUFFERING_SKINNED]->use();
				_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Mat4>         (StringID("projection_matrix", 0x92b1e336c34a1224)).set(infos.cameraInfos.data.projection);
				_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Mat4>         (StringID("view_matrix", 0xd15d560e7965726c)).set(infos.cameraInfos.view);
				_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<SamplerBuffer>(StringID("model_matrix_tbo", 0x6532aea46fc01c3a)).set(_positionBuffer);
				_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<SamplerBuffer>(StringID("bones_matrix_tbo", 0x3a7f8c7debc73024)).set(GetRenderThread()->getBonesTexture());
				auto matrixOffset = _programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Vec1>(StringID("matrixOffset", 0xb870d9a9a2c195f7));
				auto bonesOffset = _programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Vec1> (StringID("bonesOffset", 0xc8c5f289dcfef0cf));

				_positionBuffer->resetOffset();

				std::shared_ptr<Painter> painter = nullptr;
				Key<Vertices> verticesKey;

				// draw for the spot light selected
				auto &generator = toDraw->getCommandOutput();
				auto &occluders = generator._commands;
				std::size_t occluderCounter = 0;

				_positionBuffer->set((void*)(generator._datas.data()), generator._datas.size() > _maxMatrixInstancied ? _maxMatrixInstancied : generator._datas.size());

				while (occluderCounter < occluders.size())
				{
					auto &current = occluders[occluderCounter];

					Key<Painter> painterKey;
					UnConcatenateKey(current.verticeKey, painterKey, verticesKey);

					if (painterKey.isValid())
					{
						_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Vec4>(StringID("diffuse_color", 0x011da378d8e2a2c9)).set(current.material->diffuse);
						_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Sampler2D>(StringID("diffuse_map", 0x1930bc220c3b5c20)).set(current.material->diffuseTex);
						_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Vec4>(StringID("specular_color", 0x747083b1ac56a160)).set(current.material->specular);
						_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Vec1>(StringID("shininess_ratio", 0xf147b658a317675f)).set(current.material->shininess);
						_programs[PROGRAM_BUFFERING_SKINNED]->get_resource<Sampler2D>(StringID("normal_map", 0xda3297075023f6d7)).set(current.material->normalTex);

						painter = _painterManager->get_painter(painterKey);
						painter->instanciedDrawBegin(_programs[PROGRAM_BUFFERING_SKINNED]);
						matrixOffset.set(float(current.from));
						bonesOffset.set(float(current.bonesIndex));
						painter->instanciedDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_SKINNED], verticesKey, current.size);
						painter->instanciedDrawEnd();
					}
					++occluderCounter;
				}
			}
			// Important !
			// After use, we have to recycle it ! Or
			// we will leak
			toDraw->reset();
			SkinnedMeshOutput::RecycleOutput(toDraw);
		}
	}

	LFQueue<BasicCommandGeneration::MeshAndMaterialOutput*>* DeferredBasicBuffering::getMeshResultQueue()
	{
		return &_cullingResults;
	}

	LFQueue<BasicCommandGeneration::SkinnedMeshAndMaterialOutput*>* DeferredBasicBuffering::getSkinnedMeshResultQueue()
	{
		return &_skinnedCullingResults;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
