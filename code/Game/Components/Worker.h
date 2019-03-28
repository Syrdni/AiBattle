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
				entity->AddComponent<Explorer>();

			else if (msg->component == WorkerTypes::craftsman)
				entity->AddComponent<Craftsman>();

			else if (msg->component == WorkerTypes::solider)
				entity->AddComponent<Soldier>();

			entity->GetComponent<TaskPlanner>()->OnReceiveMessage(UpgradeComplete::Create());
		}
	}

};