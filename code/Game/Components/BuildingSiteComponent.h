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
				entity->AddComponent<KilnComponent>();
	
			else if (msg->component == BuildingTypes::smithy)
				entity->AddComponent<SmithyComponent>();
	
			else if (msg->component == BuildingTypes::trainingCamp)
				entity->AddComponent<TrainingCampComponent>();
	
			else if (msg->component == BuildingTypes::smelter)
				entity->AddComponent<SmelterComponent>();

		}
	}
	
	
	int buildingType;
};
