#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Messages
#include "../Messages/ProgressMsg.h"

//Components
#include "Soldier.h"
#include "Craftsman.h"
#include "Explorer.h"

class Worker : public BaseComponent
{
	ECSComponentType(Worker);
public:
	void OnReceiveMessage(const Ptr<Message>& message) {

		if (message->IsA<ProgressDone>())
		{
			auto& msg = message->DynamicCast<ProgressDone>();

			if (msg->component == WorkerTypes::explorer)
			{
				entity->AddComponent<Explorer>();
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