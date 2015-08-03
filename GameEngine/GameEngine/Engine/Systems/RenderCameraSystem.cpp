#include "RenderCameraSystem.hpp"

#include <Components/CameraComponent.hpp>
#include <Components/SpotLight.hh>
#include <Components/DirectionalLightComponent.hh>
#include <Components/Light.hh>

#include <Core/Engine.hh>
#include <Core/AScene.hh>

#include <BFC/BFCLinkTracker.hpp>
#include <BFC/BFCBlockManagerFactory.hpp>
#include <BFC/BFCCullableObject.hpp>

#include <Graphic/DRBCameraDrawableList.hpp>
#include <Graphic/BFCCullableTypes.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>

namespace AGE
{
	RenderCameraSystem::RenderCameraSystem(AScene *scene) :
		System(std::move(scene)),
		_cameras(std::move(scene)),
		_spotLights(std::move(scene)),
		_directionnalLights(std::move(scene)),
		_pointLights(std::move(scene))
	{
		_name = "Camera system";
	}

	bool RenderCameraSystem::initialize()
	{
		_cameras.requireComponent<CameraComponent>();
		_spotLights.requireComponent<SpotLightComponent>();
		_directionnalLights.requireComponent<DirectionalLightComponent>();
		_pointLights.requireComponent<PointLightComponent>();
		return (true);
	}

	void RenderCameraSystem::updateBegin(float time)
	{

	}

	void RenderCameraSystem::mainUpdate(float time)
	{
		_scene->getBfcLinkTracker()->reset();

		// check if the render thread does not already have stuff to draw
		if (AGE::GetRenderThread()->haveRenderFrameTask())
		{
			return;
		}

		AGE::GetRenderThread()->setCameraDrawList(std::shared_ptr<DRBCameraDrawableList>(nullptr));

		std::list<std::shared_ptr<DRBSpotLightDrawableList>> spotLightList;
		std::list<std::shared_ptr<DRBData>> pointLightList;


		for (auto spotEntity : _spotLights.getCollection())
		{
			auto spot = spotEntity->getComponent<SpotLightComponent>();
			auto spotDrawableList = std::make_shared<DRBSpotLightDrawableList>();
			spotDrawableList->spotLight = spot->getCullableHandle().getPtr()->getDatas();

			Frustum spotlightFrustum;
			// BIG hack :
			// @PROUT TODO -> compute the spotlight frustum
			auto hackCamEnt = *(_cameras.getCollection().begin());
			auto hackCamComp = hackCamEnt->getComponent<CameraComponent>();
			spotlightFrustum.setMatrix(hackCamComp->getProjection() * glm::inverse(hackCamEnt->getLink().getGlobalTransform()));
			// hack end

			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, spotDrawableList->meshs, spotlightFrustum);
			spotLightList.push_back(spotDrawableList);
		}
		for (auto pointLightEntity : _pointLights.getCollection())
		{
			auto point = pointLightEntity->getComponent<PointLightComponent>();
			
			pointLightList.push_back(point->getCullableHandle().getPtr()->getDatas());
		}

		for (auto cameraEntity : _cameras.getCollection())
		{
			Frustum cameraFrustum;
			auto camera = cameraEntity->getComponent<CameraComponent>();

			auto cameraList = std::make_shared<DRBCameraDrawableList>();
			cameraList->cameraInfos.data = camera->getData();
			cameraList->cameraInfos.view = glm::inverse(cameraEntity->getLink().getGlobalTransform());

			cameraFrustum.setMatrix(camera->getProjection() * cameraList->cameraInfos.view);

			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, cameraList->meshs, cameraFrustum);
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullablePointLight, cameraList->pointLights, cameraFrustum);
			cameraList->spotLights = spotLightList;
			cameraList->pointLights = pointLightList;
			AGE::GetRenderThread()->setCameraDrawList(cameraList);
		}
	}

	void RenderCameraSystem::updateEnd(float time)
	{
	}
}