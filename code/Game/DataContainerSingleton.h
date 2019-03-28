#pragma once
#include "IO/IO.h"
#include "cJSON.h"

using namespace Oryol;

struct MaterialsRequired {
	int wood;
	int charcoal;
	int ironOre;
	int ironIngot;
	int sword;
};

struct UnitInfo {
	Oryol::StringAtom name;
	int health;
	int movementSpeed;
	int upgradeTime;
	Oryol::StringAtom upgradeLocation;
	MaterialsRequired materialsRequired;
};

struct BuildingInfo {
	Oryol::StringAtom name;
	int health;
	MaterialsRequired materialsRequired;
	int buildtime;
	int size;
};

struct MaterialInfo {
	Oryol::StringAtom name;
	int timeToCreate;
	Oryol::StringAtom refineLocation;
	MaterialsRequired materialsRequired;
};

struct TileInfo {
	Oryol::StringAtom name;
	int materialCount;
	float movementSpeed;
	bool walkable;
	bool buildable;
};


class DataContainerSingleton
{
public:
	/// Returns singleton instance of DataContainerSingleton class
	static DataContainerSingleton& GetInstance() {
		static DataContainerSingleton instance;

		return instance;
	}

	/// Get map containing information about units
	const Oryol::Map<Oryol::StringAtom, UnitInfo>& GetUnitMap() {
		return unitMap;
	}

	/// Get map containing information about buildings
	const Oryol::Map<Oryol::StringAtom, BuildingInfo>& GetBuildingMap() {
		return buildingMap;
	}

	/// Get map containing information about materials
	const Oryol::Map<Oryol::StringAtom, MaterialInfo>& GetMaterialMap() {
		return materialMap;
	}

	/// Get map containing information about tiles
	const Oryol::Map<Oryol::StringAtom, TileInfo>& GetTileInfoMap() {
		return tileInfoMap;
	}

	const int& GetAttackDamage() {
		return attackDamage;
	}

	const int& GetAttackCooldown() {
		return attackCooldown;
	}

	/// Deserialize data from JSON into structs and add structs to maps
	void Deserialize(const char* data) {
		cJSON *json = cJSON_Parse(data);

		//Creates UnitInfo struct for every type of unit and stores them in a oryol map
		cJSON* units = cJSON_GetObjectItem(json, "units");
		for (int i = 0; i < cJSON_GetArraySize(units); i++)
		{
			UnitInfo u;
			cJSON* unit = cJSON_GetArrayItem(units, i);
			u.name = cJSON_GetObjectItem(unit, "name")->valuestring;
			u.health = cJSON_GetObjectItem(unit, "health")->valueint;
			u.movementSpeed = cJSON_GetObjectItem(unit, "movementSpeed")->valueint;
			u.upgradeTime = cJSON_GetObjectItem(unit, "upgradeTime")->valueint;
			u.upgradeLocation = cJSON_GetObjectItem(unit, "upgradeLocation")->valuestring;
			cJSON* reqMat = cJSON_GetObjectItem(unit, "requiredMaterials");
			u.materialsRequired.wood = cJSON_GetObjectItem(reqMat, "wood")->valueint;
			u.materialsRequired.charcoal = cJSON_GetObjectItem(reqMat, "charcoal")->valueint;
			u.materialsRequired.ironOre = cJSON_GetObjectItem(reqMat, "ironOre")->valueint;
			u.materialsRequired.ironIngot = cJSON_GetObjectItem(reqMat, "ironIngot")->valueint;
			u.materialsRequired.sword = cJSON_GetObjectItem(reqMat, "sword")->valueint;
			unitMap.Add(u.name, u);
		}

		//Creates BuildingInfo struct for every type of building and stores them in a oryol map
		cJSON* buildings = cJSON_GetObjectItem(json, "buildings");
		for (int i = 0; i < cJSON_GetArraySize(buildings); i++)
		{
			BuildingInfo b;
			cJSON* building = cJSON_GetArrayItem(buildings, i);
			b.name = cJSON_GetObjectItem(building, "name")->valuestring;
			b.health = cJSON_GetObjectItem(building, "health")->valueint;
			cJSON* reqMat = cJSON_GetObjectItem(building, "requiredMaterials");
			b.materialsRequired.wood = cJSON_GetObjectItem(reqMat, "wood")->valueint;
			b.materialsRequired.charcoal = cJSON_GetObjectItem(reqMat, "charcoal")->valueint;
			b.materialsRequired.ironOre = cJSON_GetObjectItem(reqMat, "ironOre")->valueint;
			b.materialsRequired.ironIngot = cJSON_GetObjectItem(reqMat, "ironIngot")->valueint;
			b.materialsRequired.sword = cJSON_GetObjectItem(reqMat, "sword")->valueint;
			b.buildtime = cJSON_GetObjectItem(building, "buildTime")->valueint;
			b.size = cJSON_GetObjectItem(building, "size")->valueint;
			buildingMap.Add(b.name, b);
		}

		//Creates MaterialInfo struct for every type of material and stores them in a oryol map
		cJSON* materials = cJSON_GetObjectItem(json, "materials");
		for (size_t i = 0; i < cJSON_GetArraySize(materials); i++)
		{
			MaterialInfo m;
			cJSON* material = cJSON_GetArrayItem(materials, i);
			m.name = cJSON_GetObjectItem(material, "name")->valuestring;
			m.timeToCreate = cJSON_GetObjectItem(material, "timeToCreate")->valueint;
			m.refineLocation = cJSON_GetObjectItem(material, "refineLocation")->valuestring;
			cJSON* reqMat = cJSON_GetObjectItem(material, "requiredMaterials");
			m.materialsRequired.wood = cJSON_GetObjectItem(reqMat, "wood")->valueint;
			m.materialsRequired.charcoal = cJSON_GetObjectItem(reqMat, "charcoal")->valueint;
			m.materialsRequired.ironOre = cJSON_GetObjectItem(reqMat, "ironOre")->valueint;
			m.materialsRequired.ironIngot = cJSON_GetObjectItem(reqMat, "ironIngot")->valueint;
			m.materialsRequired.sword = cJSON_GetObjectItem(reqMat, "sword")->valueint;
			materialMap.Add(m.name, m);
		}

		//Creates TileInfo struct for every type of tile and stores them in a oryol map
		cJSON* tiles = cJSON_GetObjectItem(json, "map");
		for (int i = 0; i < cJSON_GetArraySize(tiles); i++)
		{
			TileInfo t;
			cJSON* tileInfo = cJSON_GetArrayItem(tiles, i);
			t.name = cJSON_GetObjectItem(tileInfo, "name")->valuestring;
			t.materialCount = cJSON_GetObjectItem(tileInfo, "materialCount")->valueint;
			t.movementSpeed = (float)cJSON_GetObjectItem(tileInfo, "movementSpeed")->valuedouble;
			t.walkable = cJSON_GetObjectItem(tileInfo, "walkable")->valueint;
			t.buildable = cJSON_GetObjectItem(tileInfo, "buildable")->valueint;
			tileInfoMap.Add(t.name, t);
			int woodreq = buildingMap["smithy"].materialsRequired.wood;
		}

		cJSON* combat = cJSON_GetObjectItem(json, "combat");
		attackDamage = cJSON_GetObjectItem(combat, "damage")->valueint;
		attackCooldown = cJSON_GetObjectItem(combat, "cooldown")->valueint;
	}

private:
	Oryol::Map<Oryol::StringAtom, UnitInfo> unitMap;
	Oryol::Map<Oryol::StringAtom, BuildingInfo> buildingMap;
	Oryol::Map<Oryol::StringAtom, MaterialInfo> materialMap;
	Oryol::Map<Oryol::StringAtom, TileInfo> tileInfoMap;
	int attackDamage;
	int attackCooldown;


	
	/// Unused constructor
	DataContainerSingleton() {}
	/// Unused deconstructor
	~DataContainerSingleton() {}
};

