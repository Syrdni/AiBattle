#pragma once

//Singleton for the JSON file
#include "Game/DataContainerSingleton.h"

//Base component class, used for inheritance
#include "ECS/BaseComponent.h"
#include "InventoryComponent.h"


//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Used to render a model
#include "ShapeRenderer.h"

//Used to get the model for the entity
#include "Game/World/ShapeServer.h"

/**
	The Explorer component is required to discover new tiles in the level.
	Any unit may be upgraded to a explorer. The explorer may also receive
	sets of discoveries, containing resources or other level objects.

	@author Philip Stenmark
*/

class Explorer : public BaseComponent
{
	ECSComponentType(Explorer);
public:
	void OnLoad() 
	{
		if (!entity->HasComponent<InventoryComponent>())
		{
			DataContainerSingleton &instance = DataContainerSingleton::GetInstance();
			entity->SetAttribute("maxHealth", instance.GetUnitMap()["explorer"].health);
			entity->SetAttribute("health", instance.GetUnitMap()["explorer"].health);
			entity->RemoveComponent<ShapeRenderer>();
			entity->AddComponent<ShapeRenderer>(ShapeServer::GetExplorerModel(entity->GetComponent<TeamTag>()->Team));
		}
	}
};