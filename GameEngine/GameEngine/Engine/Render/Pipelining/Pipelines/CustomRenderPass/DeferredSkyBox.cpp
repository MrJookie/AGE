#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSkyBox.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>
#include <Render/OcclusionTools/DepthMap.hpp>

#define DEFERRED_SHADING_BUFFERING_VERTEX "deferred_shading/deferred_shading_skybox.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "deferred_shading/deferred_shading_skybox.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_SKYBOX = 0,
		PROGRAM_NBR
	};

	DeferredSkyBox::DeferredSkyBox(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager, std::shared_ptr<Texture2D> diffuse) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;

		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();
		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");
		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);
		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;
		_programs[PROGRAM_SKYBOX] = std::make_shared<Program>(Program(std::string("program_skybox"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		GetRenderThread()->getCube(_cube, _painterCube);
	}

	void DeferredSkyBox::renderPass(RenderPipeline const &pipeline, RenderLightList &, CameraInfos const &infos)
	{
		SCOPE_profile_gpu_i("DeferredSkybox render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredSkybox render pass");
		{
			SCOPE_profile_gpu_i("Skybox buffer");
			SCOPE_profile_cpu_i("RenderTimer", "Skybox buffer");

			//OpenGLState::glEnable(GL_CULL_FACE);
			//OpenGLState::glCullFace(GL_BACK);
			OpenGLState::glDisable(GL_BLEND);
			OpenGLState::glDisable(GL_DEPTH_TEST);
			OpenGLState::glDisable(GL_STENCIL_TEST);
		}
		{
			SCOPE_profile_gpu_i("Draw all objects");
			SCOPE_profile_cpu_i("RenderTimer", "Draw all objects");

			_programs[PROGRAM_SKYBOX]->use();
			// uniform
			_painterManager->get_painter(_painterCube)->uniqueDraw(GL_QUADS, _programs[PROGRAM_SKYBOX], Properties(), _cube);
		}
	}

}
