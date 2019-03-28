#pragma once
#include "../DataContainerSingleton.h"
// Component checks
#include "ECS/Entity.h"
// Messages
#include "Game/Messages/TileChange.h"
#include "Game/Messages/EntityRemove.h"
// Components
#include "Game/Components/Explorer.h"
#include "Game/Components/TransformComponent.h"
#include "Game/Components/ShapeRenderer.h"
#include "Game/Components/LevelVisitor.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/Discoverable.h"
#include "Game/Components/TreeComponent.h"
#include "Game/Components/IronOreComponent.h"
#include "Game/Components/InventoryComponent.h"
// World Components
#include "Game/World/ShapeServer.h"
#include "Game/World/Level.h"
// Weird Components
#include "IO/IO.h"
#include "Core/String/StringBuilder.h"
#include "Assets/Gfx/ShapeBuilder.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/random.hpp"
#include "Game/Game.h"

const float Level::TileSize = 2.0f;

Level::Level(const char* data)
{
	this->data = data;
}

void Level::OnLoad()
{
	this->numTiles = 0;
	this->width = 0;

	auto& data = DataContainerSingleton::GetInstance();

	Array<String> lines;
	StringBuilder(this->data).Tokenize("\n", lines);

	// level object positions
	Array<glm::ivec2> treePositions;
	Array<glm::ivec2> ironPositions;
	Array<glm::ivec2> castlePositions;

	int x;
	for (x = 0; x < lines.Size(); x++)
	{
		StringBuilder str = lines[x];
		for (int z = 0; z < str.Length(); z++)
		{
			// Mountain
			if (str.At(z) == 'B')
			{
				tiles.Add(Tile(x, z, false, false, 0.36f, 0.36f, 0.36f, glm::linearRand(1.5f, 2.0f), 1.0f));
			}
			// Ground
			else if (str.At(z) == 'M')
			{
				tiles.Add(Tile(x, z, true, true, 0, 0.45f, 0, 1.0f, 1.0f));
			}
			// Tree
			else if (str.At(z) == 'T')
			{
				for (int i = 0; i < 2; i++)
				{
					treePositions.Add(glm::ivec2(x * Level::TileSize, z * Level::TileSize));
				}
				tiles.Add(Tile(x, z, true, false, 0, 0.40f, 0, 1.0f, 1.0f));
			}
			// Swamp
			else if (str.At(z) == 'G')
			{
				tiles.Add(Tile(x, z, true, false, 0.44f, 0.22f, 0.11f, 0.9f, 0.5f));
			}
			// Water
			else if (str.At(z) == 'V')
			{
				tiles.Add(Tile(x, z, false, false, 0.25f, 0.5f, 0.95f, 0.7f, 1.0f));
			}
			// Iron
			else if (str.At(z) == 'I')
			{
				ironPositions.Add(glm::ivec2(x * Level::TileSize, z * Level::TileSize));
				tiles.Add(Tile(x, z, true, false, 0, 0.40f, 0, 1.0f, 1.0f));
			}
			else if (str.At(z) == 'S')
			{
				castlePositions.Add(glm::ivec2(x * Level::TileSize, z * Level::TileSize));
				tiles.Add(Tile(x, z, true, false, 0, 0.45f, 0, 1.0f, 1.0f));
			}
			else
			{
				continue;
			}
			numTiles++;
		}
	}

	for (int i = 0; i < numTiles; i++)
	{
		this->entityMap.Add(&tiles[i], Set<Ptr<Entity>>());
	}

	for (auto& treePos : treePositions)
	{
		auto tree = Game::EntityManager.CreateEntity();
		auto transform = tree->AddComponent<TransformComponent>();
		transform->SetPosition(glm::vec3(treePos.x + glm::linearRand(-TileSize, TileSize) * 0.5f, Level::TileSize, treePos.y + glm::linearRand(-TileSize, TileSize) * 0.5f));
		transform->Rotate(glm::linearRand(0, 360), glm::vec3(0, 1, 0));
		tree->AddComponent<ShapeRenderer>(ShapeServer::GetTreeModel());
		tree->AddComponent<LevelVisitor>();
		tree->AddComponent<Progress>(data.GetMaterialMap()["wood"].timeToCreate);
		tree->AddComponent<Discoverable>(Discoverable::Tree);
	}

	for (auto& ironPos : ironPositions)
	{
		auto iron = Game::EntityManager.CreateEntity();
		auto transform = iron->AddComponent<TransformComponent>();
		transform->SetPosition(glm::vec3(ironPos.x, Level::TileSize, ironPos.y));
		transform->Rotate(glm::linearRand(0, 360), glm::vec3(0, 1, 0));
		iron->AddComponent<ShapeRenderer>(ShapeServer::GetIronOreModel());
		iron->AddComponent<LevelVisitor>();
		iron->AddComponent<Progress>(data.GetMaterialMap()["ironOre"].timeToCreate);
		iron->AddComponent<Discoverable>(Discoverable::IronOre);
	}

	TeamTag::Code team = TeamTag::Team1;
	for (auto& castlePos : castlePositions)
	{
		auto castle = Game::EntityManager.CreateEntity();
		auto transform = castle->AddComponent<TransformComponent>();
		transform->SetPosition(glm::vec3(castlePos.x, Level::TileSize, castlePos.y));
		castle->AddComponent<HealthComponent>(data.GetUnitMap()["castle"].health);
		castle->AddComponent<TeamTag>(team);
		castle->AddComponent<ShapeRenderer>(ShapeServer::GetCastleModel(team));
		castle->AddComponent<LevelVisitor>();
		castle->AddComponent<InventoryComponent>();
		castle->AddComponent<Discoverable>(Discoverable::Castle);
		castle->AddComponent<Explorer>();

		this->castles.Add(castle);

		team = TeamTag::Team2;
	}

	// setup tile offsets
	this->tileOffsets = { -1, 0, 1, x - 1, x, x + 1, -x - 1, -x, -x + 1 };
	this->width = x;

	// setup draw states
	this->SetupDrawing();
}

void Level::Update()
{
	this->shaderParams.mvp = Game::GetObserver();

	for (int i = 0; i < this->numTiles; ++i)
	{
		auto& tile = this->tiles[i];
		auto& info = tile.GetDrawInfo();
		if(tile.IsVisibleToAny())
		{
			info.position.y *= 0.85f;
		}
		this->instanceData[i] = info;
	}
	Gfx::UpdateVertices(this->drawState.Mesh[1], this->instanceData, this->numTiles * sizeof(TileDrawInfo));

	Gfx::ApplyDrawState(this->drawState);
	Gfx::ApplyUniformBlock(this->shaderParams);
	Gfx::Draw(0, this->numTiles);
}


Tile* Level::GetTile(const glm::vec3& position)
{
	// convert from world position to level space
	glm::ivec3 offset = glm::round(position / TileSize);
	return this->GetTile(offset.z + offset.x * width);
}

Tile* Level::GetTile(int x, int z)
{
	return this->GetTile(z + x * width);
}

Tile* Level::GetTile(int tileIndex)
{
	o_assert2_dbg(tileIndex >= 0 && tileIndex < numTiles, "Tile out of bounds");
	return &this->tiles[tileIndex];
}

Set<Ptr<Entity>> Level::GetEntities(const glm::vec3& position)
{
	return this->entityMap[this->GetTile(position)];
}

Ptr<Entity> Level::GetCastle(int team)
{
	return this->castles[team - 1];
}

void Level::SetupDrawing()
{
	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	shapeBuilder
		.Box(TileSize * 2.0f, TileSize * 2.0f, TileSize * 2.0f, 1);

	auto instanceSetup = MeshSetup::Empty(this->numTiles, Usage::Stream);
	instanceSetup.Layout
		.Add(VertexAttr::Instance0, VertexFormat::Float4)
		.Add(VertexAttr::Instance1, VertexFormat::Float4)
		.Add(VertexAttr::Instance2, VertexFormat::Float4)
		.EnableInstancing();

	this->drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
	this->drawState.Mesh[1] = Gfx::CreateResource(instanceSetup);

	Id shader = Gfx::CreateResource(TileShader::Setup());
	auto ps = PipelineSetup::FromShader(shader);
	{
		ps.Layouts[0] = shapeBuilder.Layout;
		ps.Layouts[1] = instanceSetup.Layout;
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
		ps.BlendState.BlendEnabled = true;
		ps.BlendState.SrcFactorRGB = BlendFactor::SrcAlpha;
		ps.BlendState.DstFactorRGB = BlendFactor::OneMinusSrcAlpha;
	}
	this->drawState.Pipeline = Gfx::CreateResource(ps);
	this->instanceData = new TileDrawInfo[numTiles];
}

void Level::OnDestroy()
{
	delete[] instanceData;
}
