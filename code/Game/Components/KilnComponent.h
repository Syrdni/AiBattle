#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Components
#include "HealthComponent.h"
#include "Progress.h"

//Singleton for the JSON file
#include "../DataContainerSingleton.h"

//Used to render a model
#include "ShapeRenderer.h"

//Used to get the model for the entity
#include "../World/ShapeServer.h"
class Progress;
class BuildingSiteComponent;

class KilnComponent : public BaseComponent 
{
	ECSComponentType(KilnComponent)
public:
	void OnLoad();
};

inline void KilnComponent::OnLoad() {
	DataContainerSingleton &instance = DataContainerSingleton::GetInstance();
	entity->SetAttribute("maxHealth", instance.GetBuildingMap()["charcoalPile"].health);
	entity->SetAttribute("health", instance.GetBuildingMap()["charcoalPile"].health);
	entity->RemoveComponent<Progress>();
	entity->AddComponent<Progress>(instance.GetMaterialMap()["charcoal"].timeToCreate);
	entity->RemoveComponent<BuildingSiteComponent>();
	entity->RemoveComponent<ShapeRenderer>();
	entity->AddComponent<ShapeRenderer>(ShapeServer::GetKilnModel());
}