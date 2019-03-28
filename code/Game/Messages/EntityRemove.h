#pragma once
//Message base class
#include "ECS/Message.h"

//Entity class
#include "ECS/Entity.h"

//Tile class
#include "../World/Tile.h"

/**
	Message sent when an entity is removed from the level.
	This prevents lookup of invalid or consumed entities.

	@author Philip Stenmark
*/
class EntityRemove : public Message
{
	ECSMessageType(EntityRemove);
public:
	EntityRemove(Tile* tile, const Ptr<Entity>& entity) : Tile(tile), Entity(entity) {};

	Tile* Tile;
	Ptr<Entity> Entity;
};