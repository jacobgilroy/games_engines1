#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "PhysicsFactory.h"
#include "PhysicsController.h"
#include <btBulletDynamicsCommon.h>

namespace BGE
{
	class Assignment :
		public Game
	{
	public:
		Assignment(void);
		~Assignment(void);
		
		//declare body component:
		std::shared_ptr<GameComponent> bird1;

		//declare body part shapes:
		std::shared_ptr<PhysicsController> body;
		std::shared_ptr<PhysicsController> wing1;
		std::shared_ptr<PhysicsController> wing2;
		std::shared_ptr<PhysicsController> head;
		std::shared_ptr<PhysicsController> tail;
		std::shared_ptr<PhysicsController> ear1;
		std::shared_ptr<PhysicsController> ear2;
		std::shared_ptr<PhysicsController> beak;

		//declare constraints:
		btHingeConstraint * hinge;
		btFixedConstraint * fixed;

		float force;

		bool Initialise();
		void Update(float timeDelta);
		void MoveForward();
		void MoveBackward();
	};
}