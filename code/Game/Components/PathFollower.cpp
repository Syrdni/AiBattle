#pragma once
#include "PathFollower.h"

//Messages
#include "../Messages/PathArrive.h"
#include "../Messages/PathCancel.h"
#include "../Messages/Abort.h"
#include "../World/MessageDispatcher.h"

//Other
#include "Game/Game.h"
#include "../World/PathPlanner.h"
#include "Game/DataContainerSingleton.h"

void PathFollower::OnLoad()
{
	transform = entity->GetComponent<TransformComponent>();

	DataContainerSingleton & data = DataContainerSingleton::GetInstance();
	StringAtom type = entity->GetAttribute<Oryol::StringAtom>("UnitType");
	maxVelocity = baseVelocity * data.GetUnitMap()[type].movementSpeed;
}

void PathFollower::Update()
{
	if (!this->hasPath)
		return;

	glm::vec3 desired = currentTarget - transform->GetPosition();
	float distance = glm::length(desired);
	if (distance < 0.5f)
	{
		if (!currentPath.Empty())
		{
			this->currentTarget = currentPath.PopFront();
		}
		else
		{
			this->hasPath = false;
			transform->SetPosition(currentTarget);
			entity->OnReceiveMessage(PathArrive::Create());
		}
	}
	else
	{
		desired = glm::normalize(desired) * maxVelocity;
		force += (desired - velocity);
		force *= (1.0f / mass);
		velocity += force;

		// get speed modifier of current tile
		float speedMod = Game::World->GetComponent<Level>()->GetTile(transform->GetPosition())->GetSpeedMod();

		transform->Translate(velocity * speedMod);
		transform->SetDirection(desired);
	}
}

void PathFollower::OnReceiveMessage(const Ptr<Message>& message)
{
	if (message->IsA<PathDone>())
	{
		auto msg = message->DynamicCast<PathDone>();

		this->currentPath = msg->Path;
		if (!this->currentPath.Empty())
		{
			this->currentTarget = currentPath.PopFront();
			this->hasPath = true;
		}
		else
		{
			entity->OnReceiveMessage(PathArrive::Create());
			this->hasPath = false;
		}
		
		Game::Console.Log("Path found! (nodes: %i, entity: %i)", this->currentPath.Size(), entity->Id.SlotIndex);
	}

	if (message->IsA<PathFailed>())
	{
		auto msg = message->DynamicCast<PathFailed>();
		this->hasPath = false;

		Game::Console.Log("Path failed (entity: %i): %s", entity->Id.SlotIndex, msg->Error.AsCStr());
	}

	if (message->IsA<Abort>())
	{
		Game::Console.Log("Path cancelled with %i nodes left (entity: %i)", this->currentPath.Size(), entity->Id.SlotIndex);
		this->currentPath.Clear();
	}
}

void PathFollower::UpdateMaxVelocity()
{
	DataContainerSingleton & data = DataContainerSingleton::GetInstance();
	StringAtom type = entity->GetAttribute<Oryol::StringAtom>("UnitType");
	maxVelocity = baseVelocity * data.GetUnitMap()[type].movementSpeed;
}

