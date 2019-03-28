#pragma once
// Component checks
#include "ECS/BaseComponent.h"
#include "ECS/EntityManager.h"
// Messages
#include "Game/Messages/Deposit.h"
#include "Game/Messages/ProgressMsg.h"
#include "Game/Messages/Abort.h"
// Components to check action
#include "Game/Components/BuildBuilding.h"
#include "Game/Components/CreateItem.h"
#include "Game/Components/Upgrading.h"
#include "Game/Components/MineChop.h"
// Component to check types
#include "Game/Components/Worker.h"
#include "Game/Components/BuildingSiteComponent.h"
// Inventory (not used in all cases.)
#include "Game/Components/InventoryComponent.h"
#include "Game/Components/TeamTag.h"
#include "Game/Game.h"

/*
	Class for tracking progress of buildings, resources and upgrades.
	@author Kristoffer Lundgren, Benjamin Vesterlund
*/
class Progress : public BaseComponent
{
	ECSComponentType(Progress);
public:
	Progress(int timeRequired = 0) { this->timeRequired = timeRequired; };
	/// Started changes to true when the class receives a message to start the progress. If the entity has one of the components requierd to add to the progress it increases until it has reached the time specifies in the message
	/// When the timer is over it sends back a message to the entity that sent the start message and it in turn decides what to do with it.
	/// @author kirlun-7
	void FixedUpdate();
	void OnLoad();
	void OnReceiveMessage(const Ptr<Message>& message);
private:
	
	Ptr<Entity> returnEntity;
	int component;
	bool started = false;
	float progress; 
	int timeRequired;
	float increment = 1.0f / EntityManager::TickRate;;
	float timePassed = 0;
	Ptr<InventoryComponent> inventory;
};

inline void Progress::FixedUpdate()
{
	

	if (started)
	{
		if (returnEntity->HasComponent<MineChop>() || returnEntity->HasComponent<BuildBuilding>() || returnEntity->HasComponent<CreateItem>() || returnEntity->HasComponent<Upgrading>() )
			timePassed += increment;


		if (timePassed >= timeRequired)
		{
			
			if (entity->HasComponent<BuildingSiteComponent>())
			{
				returnEntity->OnReceiveMessage(ProgressDone::Create(entity, 0));
				returnEntity->RemoveComponent<BuildBuilding>();
				entity->GetComponent<BuildingSiteComponent>()->OnReceiveMessage(ProgressDone::Create(entity, component));
			}
			else if (entity->HasComponent<Worker>())
			{
				entity->GetComponent<Worker>()->OnReceiveMessage(ProgressDone::Create(entity, component));
			}			
			else if (entity->HasComponent<SmelterComponent>())
			{
				MaterialsToDeposit materials;
				materials.ironIngot = true;
				inventory->OnReceiveMessage(Deposit::Create(materials)); //check if TeamTag added after Progress
				returnEntity->OnReceiveMessage(ProgressDone::Create(entity, 0));
			}
			else if (entity->HasComponent<SmithyComponent>()) 
			{
				MaterialsToDeposit materials;
				materials.sword = true;
				inventory->OnReceiveMessage(Deposit::Create(materials)); //check if TeamTag added after Progress
				returnEntity->OnReceiveMessage(ProgressDone::Create(entity, 0));
			}
			else if (entity->HasComponent<KilnComponent>())
			{
				MaterialsToDeposit materials;
				materials.coal = true;
				inventory->OnReceiveMessage(Deposit::Create(materials)); //check if TeamTag added after Progress
				returnEntity->OnReceiveMessage(ProgressDone::Create(entity, 0));
			}
			else
			{
				returnEntity->OnReceiveMessage(ProgressDone::Create(entity, 0));
			}

			timePassed = 0;
			started = false;
		}
	}
}

inline void Progress::OnLoad() 
{
	if (entity->HasComponent<TeamTag>())
	{
		inventory = Game::World->GetComponent<Level>()->GetCastle(entity->GetComponent<TeamTag>()->Team)->GetComponent<InventoryComponent>();
	}
}

inline void Progress::OnReceiveMessage(const Ptr<Message>& message)
{
	if (message->IsA<StartProgress>())
	{
		if (started)
			return;
		auto& msg = message->DynamicCast<StartProgress>();
		this->returnEntity = msg->returnEntity;
		this->component = msg->component;
		this->started = true;
	}
	else if (message->IsA<Abort>())
	{
		this->started = false;
		this->returnEntity = nullptr;
	}
}


