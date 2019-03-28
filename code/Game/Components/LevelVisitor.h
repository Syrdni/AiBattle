#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Components
#include "TransformComponent.h"

//Used to render a model
#include "ShapeRenderer.h"

//Messages
#include "../Messages/TileChange.h"
#include "../Messages/EntityRemove.h"

//Game and level
#include "../World/Level.h"
#include "../Game.h"

/**
	This component is used to track an entity using its tile position offset.
	This allows for faster entity lookup in the level. The component also controls
	the visibility of the object.

	@author Philip Stenmark
*/
class LevelVisitor : public BaseComponent
{
	ECSComponentType(LevelVisitor);
public:
	inline void OnLoad()
	{
		o_assert_dbg(entity->HasComponent<TransformComponent>() && entity->HasComponent<ShapeRenderer>());

		currentTile = Game::World->GetComponent<Level>()->GetTile(entity->GetAttribute<glm::vec3>("position"));
		Game::World->OnReceiveMessage(TileChange::Create(currentTile, currentTile, entity));
	}

	inline void FixedUpdate()
	{
		// set visible flag
		entity->SetAttribute("visible", currentTile->IsVisibleToAny());

		// skip if transform is unchanged
		if (!entity->GetAttribute<bool>("transformChanged"))
		{
			return;
		}

		auto nextTile = Game::World->GetComponent<Level>()->GetTile(entity->GetAttribute<glm::vec3>("position"));

		// entity has moved, change tile offset
		if (nextTile != currentTile)
		{
			Game::World->OnReceiveMessage(TileChange::Create(currentTile, nextTile, entity));
			currentTile = nextTile;
		}
	}

	inline void OnDestroy()
	{
		// send destroy message to level
		Game::World->OnReceiveMessage(EntityRemove::Create(currentTile, entity));
	}
private:
	Tile* currentTile;
	bool visible;
};