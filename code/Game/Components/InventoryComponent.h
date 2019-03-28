#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Message used by the component
#include "../Messages/Deposit.h"

//Singleton for the JSON file
#include "Game/DataContainerSingleton.h"
//#include "Game/Game.h"

struct Inventory
{
	int wood;
	int coal;
	int ironOre;
	int ironIngot;
	int sword;
};

class InventoryComponent : public BaseComponent
{
	ECSComponentType(InventoryComponent)
public:
	void OnLoad();
	void OnDestroy();
	void OnReceiveMessage(const Ptr<Message>& message);
	Inventory getInventory()
	{
		return inventory;
	}
	//checks if enouth meaterial and removes them
	bool ReadyToBuild(MaterialsRequired materials);
private:
	///Tracks how much of each material is stored
	Inventory inventory;
};