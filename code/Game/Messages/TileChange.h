#pragma once
//Message base class
#include "ECS/Message.h"

//Level class
#include "../World/Level.h"

/**
	Message used when a entity changes tile position in the level.

	@author Philip Stenmark
*/
class TileChange : public Message
{
	ECSMessageType(TileChange);
public:
	TileChange(Tile* last, Tile* next, const Ptr<Entity>& entity) : LastTile(last), NextTile(next), Entity(entity) {};

	Tile* LastTile;
	Tile* NextTile;
	Ptr<Entity> Entity;
};