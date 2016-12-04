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
	force = 100000;

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
	glm::vec3 tailPos = glm::vec3(4, 5, 12);

	//Create the shapes for each body part:
	body = physicsFactory->CreateSphere(1.5f, bodyPos, glm::quat(), false, true);
	body->rigidBody->activate();
	wing1 = physicsFactory->CreateCapsule(0.5f, 0.75f, wing1Pos, glm::angleAxis(90.0f, glm::vec3(0,0,1)));
	wing2 = physicsFactory->CreateCapsule(0.5f, 0.75f, wing2Pos, glm::angleAxis(90.0f, glm::vec3(0, 0, 1)));
	head = physicsFactory->CreateBox(2.0f, 2.0f, 1.5f, headPos, glm::quat(), false, true);
	tail = physicsFactory->CreateCapsule(0.1f, 0.23f, tailPos, glm::angleAxis(22.0f, glm::vec3(1, 0, 0)));
	//ear1 = physicsFactory->CreateCapsule;

	//define the constraints:

	btTransform headTransform;
	btTransform bodyTransform;
	btTransform tailTransform;
	headTransform.setIdentity();
	bodyTransform.setIdentity();
	tailTransform.setIdentity();

	headTransform.setRotation(GLToBtQuat(glm::angleAxis(0.0f, glm::vec3(0, 0, 0))));
	bodyTransform.setRotation(GLToBtQuat(glm::angleAxis(0.0f, glm::vec3(0, 0, 0))));
	tailTransform.setRotation(GLToBtQuat(glm::angleAxis(0.0f, glm::vec3(0, 0, 0))));

	fixed = new btFixedConstraint(*body->rigidBody, *head->rigidBody, bodyTransform, headTransform);
	dynamicsWorld->addConstraint(fixed);

	fixed = new btFixedConstraint(*body->rigidBody, *tail->rigidBody, bodyTransform, tailTransform);
	dynamicsWorld->addConstraint(fixed);

	hinge = new btHingeConstraint(*body->rigidBody, *wing1->rigidBody, btVector3(1.5, 0, 0), btVector3(0, 0.75, 0), btVector3(0, 0, 1), btVector3(0, 0, 1), true);
	hinge->enableAngularMotor(true, 30000, 0.25f);
	hinge->setAxis(btVector3(0, 0, 1));
	hinge->setLimit(-1, 1);
	dynamicsWorld->addConstraint(hinge);

	hinge = new btHingeConstraint(*body->rigidBody, *wing2->rigidBody, btVector3(-1.5, 0, 0), btVector3(0, -0.75, 0), btVector3(0, 0, 1), btVector3(0, 0, 1), true);
	hinge->enableAngularMotor(true, -30000, 0.25f);
	hinge->setAxis(btVector3(0, 0, 1));
	hinge->setLimit(-1, 1);
	dynamicsWorld->addConstraint(hinge);

	/*
	bird1->Attach(make_shared<Steerable3DController>(body));
	bird1->Attach(body);

	Attach(bird1);
	*/

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
		MoveForward();
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		MoveBackward();
	}

	Game::Update(timeDelta);
}

void Assignment::MoveForward()
{
	btVector3 pos = GLToBtVector(body->transform->position);

	//MOVE FORWARD
	body->rigidBody->setFriction(0);
	//body->rigidBody->applyCentralForce(GLToBtVector(body->transform->basisLook * force));
	body->rigidBody->applyForce(btVector3(0, 0, 50), pos);
}

void Assignment::MoveBackward()
{
	btVector3 pos = GLToBtVector(body->transform->position);

	//MOVE BACKWARDS
	body->rigidBody->setFriction(0);
	//body->rigidBody->applyCentralForce(GLToBtVector(-body->transform->basisLook * force));
	body->rigidBody->applyForce(btVector3(0, 0, -50), pos);
}