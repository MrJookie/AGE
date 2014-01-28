#ifndef   __FP_CONTROLLER_HPP__
# define  __FP_CONTROLLER_HPP__

#include <Components/Component.hh>
#include "Core/Engine.hh"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Context/SdlContext.hh"
#include <Managers/BulletDynamicManager.hpp>
#include <array>

namespace Component
{
	ATTRIBUTE_ALIGNED16(struct) FPController : public ComponentBase<FPController>
	{
		enum CONTROLS
		{
			FORWARD = 0,
			BACKWARD,
			LEFT,
			RIGHT,
			JUMP,
			RUN
		};
		BT_DECLARE_ALIGNED_ALLOCATOR();
		FPController();
		virtual ~FPController();
		void init();
		virtual void reset();
		void clear();
		btKinematicCharacterController &getController();
		btGhostObject &getGhost();
		btConvexShape &getShape();

		void setKey(CONTROLS k, unsigned int key);
		void resetControls();

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new FPController();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
		}

		template <typename Archive>
		void load(Archive &ar)
		{
		}

		// !Serialization
		////
		//////


		float yOrientation;
		float forwardWalkSpeed;
		float backwardWalkSpeed;
		float forwardRunSpeed;
		float backwardRunSpeed;
		float sideWalkSpeed;
		float sideRunSpeed;
		float rotateXSpeed;
		float rotateYSpeed;
		float jumpSpeed;
		float jumpHeight;
		bool canJump;
		bool canRun;
		std::array<unsigned int, 6> keys;
		std::array<bool, 6> controls;

	private:
		btKinematicCharacterController *_controller;
		btPairCachingGhostObject *_ghost;
		btConvexShape *_shape;
	};
}

#endif    //__FP_CONTROLLER_HPP__