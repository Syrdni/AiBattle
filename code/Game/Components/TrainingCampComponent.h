#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Used to render a model
#include "ShapeRenderer.h"
#include "BuildingSiteComponent.h"

//Used to get the model for the entity
#include "../World/ShapeServer.h"

class TrainingCampComponent : public BaseComponent {
	ECSComponentType(TrainingCampComponent)
public:
	void OnLoad();

};

inline void TrainingCampComponent::OnLoad() {
	DataContainerSingleton &instance = DataContainerSingleton::GetInstance();
	entity->SetAttribute("maxHealth", instance.GetBuildingMap()["trainingCamp"].health);
	entity->SetAttribute("health", instance.GetBuildingMap()["trainingCamp"].health);	
	entity->RemoveComponent<BuildingSiteComponent>();
	entity->RemoveComponent<ShapeRenderer>();
	entity->AddComponent<ShapeRenderer>(ShapeServer::GetTrainingCampModel(entity->GetComponent<TeamTag>()->Team));
}
