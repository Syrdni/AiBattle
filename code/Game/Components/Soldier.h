#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Singleton for the JSON file
#include "Game/DataContainerSingleton.h"

//Components
#include "MineChop.h"

//Messages
#include "Game/Messages/AttackCooldown.h"

//Used to render a model
#include "ShapeRenderer.h"

//Used to get the model for the entity
#include "../World/ShapeServer.h"

#include <map>
typedef std::map<char*,char*> PyMessage;

class Soldier : public BaseComponent
{
	ECSComponentType(Soldier);
public:
	inline void OnLoad() {
		DataContainerSingleton &instance = DataContainerSingleton::GetInstance();
		entity->SetAttribute("maxHealth", instance.GetUnitMap()["soldier"].health);
		entity->SetAttribute("health", instance.GetUnitMap()["soldier"].health);
		entity->RemoveComponent<ShapeRenderer>();
		entity->AddComponent<ShapeRenderer>(ShapeServer::GetSoldierModel(entity->GetComponent<TeamTag>()->Team));

		if (entity->HasComponent<MineChop>())
			entity->RemoveComponent<MineChop>();
	};

	inline void OnReceiveMessage(const Ptr<Message>& message) 
	{
		if (message->IsA<AttackCooldown>())
		{
			Game::Console.Log("Entity %i Can attack", entity->Id.SlotIndex);
			canAttack = true;

			PyMessage msg;
			msg["type"] = "canAttack";

			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());
			SendMessage(msg);
		}
	};

	bool canAttack = true;

private:
	void SendMessage(PyMessage msg);
};