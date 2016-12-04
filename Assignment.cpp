#include "Assignment.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"

using namespace BGE;

Assignment::Assignment(void)
{
	
}

Assignment::~Assignment(void)
{

}

bool Assignment::Initialise()
{
	speed = 1000;

	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();
	//dynamicsWorld->setGravity(btVector3(0, -9, 0));

	//define position variables:
	glm::vec3 bodyPos = glm::vec3(4, 5, 10);
	glm::vec3 wing1Pos = glm::vec3(7, 5, 10);
	glm::vec3 wing2Pos = glm::vec3(1, 5, 10);
	glm::vec3 headPos = glm::vec3(4, 5, 8);
	glm::vec3 tailPos = glm::vec3(4, 5, 11);

	//Create the shapes for each body part:
	body = physicsFactory->CreateSphere(1.5f, bodyPos, glm::quat(), false, true);
	wing1 = physicsFactory->CreateCapsule(0.5f, 0.75f, wing1Pos, glm::angleAxis(90.0f, glm::vec3(0,0,1)));
	wing2 = physicsFactory->CreateCapsule(0.5f, 0.75f, wing2Pos, glm::angleAxis(90.0f, glm::vec3(0, 0, 1)));
	head = physicsFactory->CreateBox(2.0f, 2.0f, 1.5f, headPos, glm::quat(), false, true);
	tail = physicsFactory->CreateCapsule(0.1f, 0.23f, tailPos, glm::angleAxis(45.0f, glm::vec3(1,0,0)));
	//ear1 = physicsFactory->CreateCapsule;

	//define the constraints:

	btTransform headTransform;
	btTransform bodyTransform;
	headTransform.setIdentity();
	bodyTransform.setIdentity();

	// You have to make the x axis rotate to the axis you want to slide
	headTransform.setRotation(GLToBtQuat(glm::angleAxis(0.0f, glm::vec3(0, 0, 0))));
	bodyTransform.setRotation(GLToBtQuat(glm::angleAxis(0.0f, glm::vec3(0, 0, 0))));

	fixed = new btFixedConstraint(*body->rigidBody, *head->rigidBody, bodyTransform, headTransform);


	hinge = new btHingeConstraint(*body->rigidBody, *wing1->rigidBody, btVector3(1.5, 0, 0), btVector3(0, 0.75, 0), btVector3(0, 0, 1), btVector3(0, 0, 1), true);
	hinge->enableAngularMotor(true, 50000, 0.5f);
	dynamicsWorld->addConstraint(hinge);

	hinge = new btHingeConstraint(*body->rigidBody, *wing2->rigidBody, btVector3(-1.5, 0, 0), btVector3(0, -0.75, 0), btVector3(0, 0, 1), btVector3(0, 0, 1), true);
	hinge->enableAngularMotor(true, -50000, 0.5f);
	dynamicsWorld->addConstraint(hinge);

	/*
	bird1->Attach(make_shared<Steerable3DController>(body));
	bird1->Attach(body);

	Attach(bird1);
	*/

	//****Make other joints*********************************************************************

	//fixed = new btFixedConstraint(*body->rigidBody, *head->rigidBody, ...);

	if (!Game::Initialise())
	{
		return false;
	}

	camera->transform->position = glm::vec3(0, 10, 20);

	return true;
}

void Assignment::Update(float timeDelta)
{
	//...

	if (keyState[SDL_SCANCODE_LEFT])
	{
		//ROTATE LEFT
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		//ROTATE RIGHT
	}
	if (keyState[SDL_SCANCODE_UP])
	{
		//MOVE FORWARD
		body->transform->Walk(timeDelta * speed);
		//steer->AddForce(glm::vec3(0, 0, 1) * speed * timeDelta);
		
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		//MOVE BACKWARDS
		body->transform->Walk(-timeDelta * speed);
	}

	Game::Update(timeDelta);
}