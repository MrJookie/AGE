#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <Utils/Containers/LFQueue.hpp>

#include "Render\Pipelining\Prepare\MeshBufferingPrepare.hpp"

namespace AGE
{
	/*
	Contains all infos needed by spotlight render pass
	*/

	struct DRBCameraDrawableList;

	class SpotlightRenderInfos
	{
	public:
		typedef BasicCommandGeneration::MeshShadowOutput MeshOutput;
		typedef BasicCommandGeneration::SkinnedShadowOutput SkinnedOutput;

		struct Camera
		{
			glm::mat4 projection;
			glm::mat4 view;
		};

		struct Spotlight
		{
			glm::vec3 position;
			glm::vec3 attenuation;
			glm::vec3 direction;
			glm::vec3 color;
			glm::mat4 matrix;
			float     cutOff;
			float     exponent;
			MeshOutput* meshs;
			SkinnedOutput* skinnedMeshs;
		};

		// we have to put that in pool !!!
		class Output
		{
		public:
			void setCameraInfos(const glm::mat4 &projection, const glm::mat4 &view);
			const Spotlight &setSpotlightInfos(
				const glm::vec3 &position,
				const glm::vec3 &attenuation,
				const glm::vec3 &direction,
				const glm::vec3 &color,
				const glm::mat4 &matrix,
				const float &cutOff,
				const float &exponent);
			const std::vector<Spotlight> &getSpots() const { return _spots; }
			const std::vector<Camera> &getCameras() const { return _cameras; }
		private:
			std::vector<Spotlight> _spots;
			std::vector<Camera> _cameras;
		};

		Output createOutput();

		SpotlightRenderInfos();

		const std::vector<Camera> &getCameras() const;
		const std::vector<Spotlight> &getSpotlights() const;
		
		// Call that after render preparation
		void computeRenderInfos(const DRBCameraDrawableList &infos);
		
		// Call that when the rendering is done
		void clearRenderInfos(const DRBCameraDrawableList &infos);

	private:
		LFQueue<BasicCommandGeneration::MeshShadowOutput*>          _cullingResults;
		LFQueue<BasicCommandGeneration::SkinnedShadowOutput*>       _skinnedCullingResults;

		std::vector<Spotlight> _spotlights;
		std::vector<Camera> _cameras;
	};
}