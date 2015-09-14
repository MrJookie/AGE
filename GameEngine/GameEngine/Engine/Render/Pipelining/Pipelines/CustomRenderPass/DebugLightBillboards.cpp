#include <Render/Pipelining/Pipelines/CustomRenderPass/DebugLightBillboards.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderPipeline.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Configuration.hpp>

#include "Graphic\DRBPointLightData.hpp"
#include "Graphic\DRBCameraDrawableList.hpp"
#include "Graphic\DRBSpotLightData.hpp"

#define DEFERRED_SHADING_DEBUG_LIGHT_VERTEX "../../Shaders/deferred_shading/deferred_shading_debug_light.vp"
#define DEFERRED_SHADING_DEBUG_LIGHT_FRAG "../../Shaders/deferred_shading/deferred_shading_debug_light.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BUFFERING_LIGHT = 0,
		PROGRAM_NBR
	};

	DebugLightBillboards::DebugLightBillboards(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse,
		std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		Key<Painter> quadPainterKey;
		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);

		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;

		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_programs.resize(PROGRAM_NBR);

		_programs[PROGRAM_BUFFERING_LIGHT] = std::make_shared<Program>(Program(std::string("debug_lights"),
		{
			std::make_shared<UnitProg>(DEFERRED_SHADING_DEBUG_LIGHT_VERTEX, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(DEFERRED_SHADING_DEBUG_LIGHT_FRAG, GL_FRAGMENT_SHADER)
		}));
	}

	void DebugLightBillboards::renderPass(const DRBCameraDrawableList &infos)
	{
		//@PROUT TODO
		{
			SCOPE_profile_gpu_i("DeferredDebugBuffering render pass");
			SCOPE_profile_cpu_i("RenderTimer", "DeferredDebugBuffering render pass");

			OpenGLState::glDisable(GL_CULL_FACE);
			OpenGLState::glDepthMask(GL_TRUE);
			OpenGLState::glDepthFunc(GL_LESS);
			OpenGLState::glDisable(GL_BLEND);
			OpenGLState::glDisable(GL_STENCIL_TEST);
			OpenGLState::glEnable(GL_DEPTH_TEST);
			OpenGLState::glDepthMask(GL_TRUE);
			{
				SCOPE_profile_gpu_i("Overhead Pipeline");
				SCOPE_profile_cpu_i("RenderTimer", "Overhead Pipeline");
				_programs[PROGRAM_BUFFERING_LIGHT]->use();
				_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Mat4>("projection_matrix").set(infos.cameraInfos.data.projection);
				_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Mat4>("view_matrix").set(infos.cameraInfos.view);
			}

			{
				SCOPE_profile_gpu_i("Render debug sprite lights");
				SCOPE_profile_cpu_i("RenderTimer", "Render debug sprite lights");
				auto &pointLightList = (std::list<std::shared_ptr<DRBPointLightData>>&)(infos.pointLights);
				_quadPainter->uniqueDrawBegin(_programs[PROGRAM_BUFFERING_LIGHT]);
				for (auto &pl : pointLightList)
				{
					_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_LIGHT], pl->globalProperties, _quadVertices);
				}
				_quadPainter->uniqueDrawEnd();
			}
			{
				SCOPE_profile_gpu_i("Render debug sprite lights");
				SCOPE_profile_cpu_i("RenderTimer", "Render debug sprite lights");
				auto &spotLightList = infos.spotLights;
				_quadPainter->uniqueDrawBegin(_programs[PROGRAM_BUFFERING_LIGHT]);
				for (auto &pl : spotLightList)
				{
					auto &spotlight = (std::shared_ptr<DRBSpotLightData>&)(pl->spotLight);
					_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_LIGHT], spotlight->globalProperties, _quadVertices);
				}
				_quadPainter->uniqueDrawEnd();
			}
		}
	}
}
