#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Singleton for the JSON file
#include "Game/DataContainerSingleton.h"

//Components
#include "BuildingSiteComponent.h"
#include "HealthComponent.h"
#include "Progress.h"
//Used to render a model
#include "ShapeRenderer.h"

//Used to get the model for the entity
#include "../World/ShapeServer.h"
/*
	Component for creating a Smithy from a building site.
	@author Kristoffer Lungren, Benjamin Vesterlund
*/
class Progress;
class SmithyComponent : public BaseComponent 
{
	ECSComponentType(SmithyComponent);
public:
	void OnLoad();
};

inline void SmithyComponent::OnLoad()
{
	DataContainerSingleton& instance = DataContainerSingleton::GetInstance();
	entity->SetAttribute("maxHealth", instance.GetBuildingMap()["smithy"].health);
	entity->SetAttribute("health", instance.GetBuildingMap()["smithy"].health);
	entity->RemoveComponent<Progress>();
	entity->AddComponent<Progress>(instance.GetMaterialMap()["sword"].timeToCreate);
	entity->RemoveComponent<BuildingSiteComponent>();
	entity->RemoveComponent<ShapeRenderer>();
	entity->AddComponent<ShapeRenderer>(ShapeServer::GetSmithyModel());
}
