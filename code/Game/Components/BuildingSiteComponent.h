#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Components
#include "KilnComponent.h"
#include "SmithyComponent.h"
#include "TrainingCampComponent.h"
#include "SmelterComponent.h"
#include "Explorer.h"
#include "Craftsman.h"
#include "Soldier.h"

//Message that is used by this component
#include "../Messages/ProgressMsg.h"
/*
	Component that recieves a message
	@author Kristoffer Lungren, Benjamin Vesterlund
*/
class BuildingSiteComponent : public BaseComponent 
{
	ECSComponentType(BuildingSiteComponent);
public:

	BuildingSiteComponent(int buildingType) { this->buildingType = buildingType; };

	void OnLoad() 
	{
		entity->RegisterAttribute("buildingType", &buildingType, Attribute::Read);
	}

	void OnReceiveMessage(const Ptr<Message>& message) {
	
		if (message->IsA<ProgressDone>())
		{
			auto& msg = message->DynamicCast<ProgressDone>();
	
			if (msg->component == BuildingTypes::kiln)
			{
				entity->AddComponent<KilnComponent>();
				if (entity->GetComponent<TeamTag>()->Team == 1)
					Game::units[8]++;
				else
					Game::units[12]++;

			}


			else if (msg->component == BuildingTypes::smithy)
			{
				entity->AddComponent<SmithyComponent>();
				if (entity->GetComponent<TeamTag>()->Team == 1)
					Game::units[10]++;
				else
					Game::units[14]++;
			}


			else if (msg->component == BuildingTypes::trainingCamp)
			{
				entity->AddComponent<TrainingCampComponent>();
				if (entity->GetComponent<TeamTag>()->Team == 1)
					Game::units[11]++;
				else
					Game::units[15]++;
			}


			else if (msg->component == BuildingTypes::smelter)
			{
				entity->AddComponent<SmelterComponent>();
				if (entity->GetComponent<TeamTag>()->Team == 1)
					Game::units[9]++;
				else
					Game::units[13]++;
			}
		}
	}
	
	
	int buildingType;
};
