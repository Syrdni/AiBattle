#pragma once
#include "ECS/Entity.h"

#define NONE_VISIBLE 0
#define WALK_MASK 1
#define BUILD_MASK 2
#define ANY_VISIBLE 3

/// contains rendering info
struct TileDrawInfo
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 scale;
};

class Tile
{
public:
	/// constructs a tile from raw data
	Tile(int x, int z, bool walkable, bool buildable, float r, float g, float b, float height = 0.0f, float speedMod = 1.0f);

	/// checks if the given entity may traverse the tile
	bool CanPass(const Ptr<Entity>& entity) const;

	/// retrieves the drawing data
	TileDrawInfo& GetDrawInfo();

	bool IsBuildable() const;

	/// checks if the tile is visible
	bool IsVisibleToAny() const;

	/// retrieves the speed modifier
	float GetSpeedMod() const;

	int x;
	int z;

private:
	friend class Level;
	TileDrawInfo drawInfo;
	uint8_t visibleBits = 0;
	uint8_t featureBits = 0;
	float speedMod;
};
