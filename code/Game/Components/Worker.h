#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Messages
#include "../Messages/ProgressMsg.h"

//Components
#include "Soldier.h"
#include "Craftsman.h"
#include "Explorer.h"
#include "PathFollower.h"

class Worker : public BaseComponent
{
	ECSComponentType(Worker);

private:
	Oryol::StringAtom UnitType;

public:
	void OnLoad() {
		UnitType = "worker";
		entity->RegisterAttribute(Oryol::StringAtom("UnitType"), &UnitType, Attribute::ReadWrite);
	};

	void OnReceiveMessage(const Ptr<Message>& message) {

		if (message->IsA<ProgressDone>())
		{
			auto& msg = message->DynamicCast<ProgressDone>();

			// Upgrade worker to chosen type and update movementspeed

			if (msg->component == WorkerTypes::explorer)
			{
				entity->AddComponent<Explorer>();
				entity->SetAttribute(Oryol::StringAtom("UnitType"), Oryol::StringAtom("explorer"));
				entity->GetComponent<PathFollower>()->UpdateMaxVelocity();

				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[1]++;
					Game::units[0]--;
				}
				else
				{
					Game::units[5]++;
					Game::units[4]--;
				}
			}


			else if (msg->component == WorkerTypes::craftsman)
			{
				entity->AddComponent<Craftsman>();
				entity->SetAttribute(Oryol::StringAtom("UnitType"), Oryol::StringAtom("craftsman"));
				entity->GetComponent<PathFollower>()->UpdateMaxVelocity();

				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[2]++;
					Game::units[0]--;
				}
				else
				{
					Game::units[6]++;
					Game::units[4]--;
				}
			}


			else if (msg->component == WorkerTypes::solider)
			{
				entity->AddComponent<Soldier>();
				entity->SetAttribute(Oryol::StringAtom("UnitType"), Oryol::StringAtom("soldier"));
				entity->GetComponent<PathFollower>()->UpdateMaxVelocity();

				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[3]++;
					Game::units[0]--;
				}
				else
				{
					Game::units[7]++;
					Game::units[4]--;
				}
			}

			entity->GetComponent<TaskPlanner>()->OnReceiveMessage(UpgradeComplete::Create());
		}
	}

};