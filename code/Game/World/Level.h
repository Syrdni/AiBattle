#pragma once
//Component checks
#include "ECS/BaseComponent.h"

//Components
#include "../Components/TeamTag.h"

//Oryol containers
#include "Core/Containers/StaticArray.h"
#include "Core/Containers/Map.h"
#include "Core/Containers/Set.h"

//Graphics
#include "Gfx/Gfx.h"
#include "Shaders/shaders.h"

//Other 
#include "Tile.h"
//#include "AICore.h"

class Level : public BaseComponent
{
	ECSComponentType(Level);
public:
	/// creates a level from text file
	Level(const char* data);

	/// sets up rendering
	void OnLoad();

	/// draws all tiles to the screen
	void Update();

	/// listens for incoming messages
	void OnReceiveMessage(const Ptr<Message>& message); // located in AICore.h sry


	/// retrieve a tile by position
	Tile* GetTile(const glm::vec3& position);

	/// retrieve a tile by coordinates
	Tile* GetTile(int x, int z);

	/// retrieve a tile by index
	Tile* GetTile(int tileIndex);

	/// lookup and fetch all entities at the given position
	Set<Ptr<Entity>> GetEntities(const glm::vec3& position);

	/// lookup and fetch all entities of given type at the position
	template<typename T> 
	Set<Ptr<Entity>> GetEntities(const glm::vec3& position);

	/// retrieve a castle entity by team
	Ptr<Entity> GetCastle(int team);

	/// destroys resources
	void OnDestroy();

	/// tile size
	static const float TileSize;

	/// selected team
	static int SelectedTeam;
private:
	void SetupDrawing();

	Array<int> tileOffsets;

	const char* data;
	Array<Tile> tiles;
	Array<Ptr<Entity>> castles;
	Map<Tile*, Set<Ptr<Entity>>> entityMap;

	int width;

	// rendering
	DrawState drawState;
	TileShader::params shaderParams;
	TileDrawInfo* instanceData;
	int numTiles;	
};

template<typename T>
inline Set<Ptr<Entity>> Level::GetEntities(const glm::vec3 & position)
{
	Set<Ptr<Entity>> result;
	auto& set = this->GetEntities(position);
	for (auto& e : set)
	{
		if (e->HasComponent<T>())
			result.Add(e);
	}
	return result;
}
