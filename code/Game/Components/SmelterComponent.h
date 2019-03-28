#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Used to render a model
#include "ShapeRenderer.h"

//Used to get the model for the entity
#include "../World/ShapeServer.h"

#include "Progress.h"
#include "BuildingSiteComponent.h"
/*
	Component for creating a smelter from a building site.
	@author Kristoffer Lungren, Benjamin Vesterlund
*/
class Progress;
class SmelterComponent : public BaseComponent {
	ECSComponentType(SmelterComponent)
public:
	void OnLoad();
};

inline void SmelterComponent::OnLoad() {
	DataContainerSingleton &instance = DataContainerSingleton::GetInstance();
	entity->SetAttribute("maxHealth", instance.GetBuildingMap()["smelter"].health);
	entity->SetAttribute("health", instance.GetBuildingMap()["smelter"].health);
	entity->RemoveComponent<Progress>();
	entity->AddComponent<Progress>(1);//instance.GetMaterialMap()["ironIngot"].timeToCreate);
	entity->RemoveComponent<BuildingSiteComponent>();
	entity->RemoveComponent<ShapeRenderer>();
	entity->AddComponent<ShapeRenderer>(ShapeServer::GetSmelterModel());
};
