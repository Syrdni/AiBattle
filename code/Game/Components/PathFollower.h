#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component its attached to
#include "ECS/Entity.h"

//Componenets
#include "TransformComponent.h"

/**
	Follows a given path using simple steering.
	A path may be received using a path request to the path planner.

	@author Philip Stenmark
*/
class PathFollower : public BaseComponent
{
	ECSComponentType(PathFollower);
public:
	void OnLoad();
	void Update();
	void OnReceiveMessage(const Ptr<Message>& message);
	void UpdateMaxVelocity();
private:
	glm::vec3 velocity;
	glm::vec3 force;
	float mass = 4.0f;
	float maxVelocity = 0.0033f;
	float baseVelocity = 0.0033f;

	// the entity transform
	Ptr<TransformComponent> transform;

	// path information
	Array<glm::vec3> currentPath;
	bool hasPath;
	glm::vec3 currentTarget;

};