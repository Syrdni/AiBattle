#pragma once

// Component checks
#include "ECS/BaseComponent.h"
#include "ECS/Entity.h"
// Singelton of the Jsonfile
#include "../DataContainerSingleton.h"
// Messages
#include "Game/Messages/ProgressMsg.h"
#include "Game/Messages/UpgradeComplete.h"
// Components
#include "MineChop.h"
#include "TaskPlanner.h"
#include "ShapeRenderer.h"
// World Components
#include "../World/ShapeServer.h"


class Craftsman : public BaseComponent
{
	ECSComponentType(Craftsman);
public:

	void OnLoad() {
		DataContainerSingleton &instance = DataContainerSingleton::GetInstance();
		entity->SetAttribute("maxHealth", instance.GetUnitMap()["craftsman"].health);
		entity->SetAttribute("health", instance.GetUnitMap()["craftsman"].health);

		entity->RemoveComponent<ShapeRenderer>();
		entity->AddComponent<ShapeRenderer>(ShapeServer::GetCraftsmanModel(entity->GetComponent<TeamTag>()->Team));

		if (entity->HasComponent<MineChop>())
			entity->RemoveComponent<MineChop>();
	}

	void OnReceiveMessage(const Ptr<Message>& message)
	{

	};
};
