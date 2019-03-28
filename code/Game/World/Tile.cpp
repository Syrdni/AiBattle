
#include "Tile.h"

//The level
#include "Level.h"

//Unit Components
#include "../Components/Explorer.h"
#include "../Components/TeamTag.h"

//Glm
#include "glm/gtc/random.hpp"

Tile::Tile(int x, int z, bool walkable, bool buildable, float r, float g, float b, float height, float speedMod)
{
	this->x = x;
	this->z = z;
	this->speedMod = speedMod;

	// hide tiles by default
	visibleBits = NONE_VISIBLE;

	// set features
	featureBits |= (walkable ? WALK_MASK : 0);
	featureBits |= (buildable ? BUILD_MASK : 0);

	// set draw info
	drawInfo.position = glm::vec4(x * Level::TileSize * 2.0f, -glm::linearRand(1.0f, 5.0f), z * Level::TileSize * 2.0f, 1.0f);
	drawInfo.color = glm::vec4(r, g, b, 1.0f);
	drawInfo.scale = glm::vec4(1.0f, height, 1.0f, 1.0f);
}

bool Tile::CanPass(const Ptr<Entity>& entity) const
{
	o_assert2_dbg(entity->HasComponent<TeamTag>(), "Entity must have a team tag");
	// can walk on any hidden tile if explorer
	if (entity->HasComponent<Explorer>())
	{
		return (featureBits & WALK_MASK) != 0;
	}
	// otherwise, only walk if tile is visible for current team
	return (featureBits & WALK_MASK) != 0 && (visibleBits & entity->GetComponent<TeamTag>()->Team) != 0;
}

TileDrawInfo& Tile::GetDrawInfo()
{
	return this->drawInfo;
}

bool Tile::IsBuildable() const
{
	return (featureBits & BUILD_MASK) != 0;
}

bool Tile::IsVisibleToAny() const
{
	return (visibleBits & ANY_VISIBLE) != 0;
}

bool Tile::IsVisibleToTeam(int teamTag) const
{
	return (visibleBits & teamTag) != 0;
}

float Tile::GetSpeedMod() const
{
	return this->speedMod;
}