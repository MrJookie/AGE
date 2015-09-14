#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredShadowBuffering.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderPipeline.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>
#include <Render/OcclusionTools/DepthMap.hpp>
#include <Render/Pipelining/Pipelines/PipelineTools.hh>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphic/DRBCameraDrawableList.hpp"
#include "Graphic/DRBSpotLightData.hpp"
#include "Graphic/DRBMeshData.hpp"

#define DEFERRED_SHADING_SHADOW_BUFFERING_VERTEX "deferred_shading/deferred_shading_get_shadow_buffer.vp"
#define DEFERRED_SHADING_SHADOW_BUFFERING_FRAG "deferred_shading/deferred_shading_get_shadow_buffer.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BUFFERING = 0,
		PROGRAM_NBR
	};

	DeferredShadowBuffering::DeferredShadowBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;
		_programs.resize(PROGRAM_NBR);
		auto confManager = GetEngine()->getInstance<ConfigurationManager>();
		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");
		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);
		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SHADOW_BUFFERING_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SHADOW_BUFFERING_FRAG;
		_programs[PROGRAM_BUFFERING] = std::make_shared<Program>(Program(std::string("program_shadow_buffering"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
	}

	void DeferredShadowBuffering::renderPass(const DRBCameraDrawableList &infos)
	{
		//@PROUT
		SCOPE_profile_gpu_i("DeferredShadowBuffering render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredShadowBuffering render pass");

		OpenGLState::glEnable(GL_CULL_FACE);
		OpenGLState::glCullFace(GL_FRONT);
		OpenGLState::glDepthMask(GL_TRUE);
		OpenGLState::glDepthFunc(GL_LEQUAL);
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glDepthMask(GL_TRUE);
		OpenGLState::glDepthFunc(GL_LESS);

		_programs[PROGRAM_BUFFERING]->use();

		// handle the number of sample
		if (_depthBuffers.size() < infos.spotLights.size())
		{
			std::size_t count = infos.spotLights.size() - _depthBuffers.size();
			for (int index = 0; index < count; ++index)
			{
				_depthBuffers.push_back(createRenderPassOutput<Texture2D>(_frame_buffer.width(), _frame_buffer.height(), GL_DEPTH24_STENCIL8, true));
				_depthBuffers.back()->bind();
				_depthBuffers.back()->parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				_depthBuffers.back()->parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				_depthBuffers.back()->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				_depthBuffers.back()->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				_depthBuffers.back()->parameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			}
		}
		else if (_depthBuffers.size() > infos.spotLights.size())
		{
			std::size_t count = _depthBuffers.size() - infos.spotLights.size();
			for (int index = 0; index < count; ++index)
			{
				_depthBuffers.pop_back();
			}
		}
		// start to render to texture for each depth map
		auto it = _depthBuffers.begin();
		for (auto &spotLightPtr : infos.spotLights)
		{
			SCOPE_profile_gpu_i("Spotlight pass");
			SCOPE_profile_cpu_i("RenderTimer", "Spotlight pass");

			DRBSpotLightData *spotlight = (DRBSpotLightData*)(spotLightPtr->spotLight.get());

			glViewport(0, 0, _frame_buffer.width(), _frame_buffer.height());
			_frame_buffer.attachment(*(*it), GL_DEPTH_STENCIL_ATTACHMENT);
			glClear(GL_DEPTH_BUFFER_BIT);
			_programs[PROGRAM_BUFFERING]->registerProperties(spotlight->globalProperties);
			_programs[PROGRAM_BUFFERING]->updateProperties(spotlight->globalProperties);

			// draw for the spot light selected
			for (auto &meshPaint : spotLightPtr->meshs)
			{
				auto mesh = (DRBMeshData*)(meshPaint.get());

				//temporary
				//todo, do not spawn entity while mesh is not loaded
				//currently it's not safe, because the paiter key can be invalid
				//during the first frames
				if (mesh->getPainterKey().isValid())
				{
					Painter *painter = _painterManager->get_painter(mesh->getPainterKey()).get();
					painter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], mesh->globalProperties, mesh->getVerticesKey());
				}
			}
			spotlight->shadowMap = *it;
			++it;
		}
	}

}