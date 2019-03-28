#include "InventoryComponent.h"
#include "Game/Game.h"
void InventoryComponent::OnLoad()
{
	inventory.wood = 0;
	inventory.coal = 0;
	inventory.ironOre = 0;
	inventory.ironIngot = 0;
	inventory.sword = 0;
}

void InventoryComponent::OnDestroy() {
	Game::running = false;
	Game::Console.Log("Team %i won", 3 - entity->GetAttribute<int>("team"));
}

void InventoryComponent::OnReceiveMessage(const Ptr<Message>& message)
{
	//Checks which material is being deposited and increases that material with one
	if (message->IsA<Deposit>())
	{
		auto msg = message->DynamicCast<Deposit>();

		if (msg->materialsToDeposit.wood)
			this->inventory.wood++;

		else if (msg->materialsToDeposit.coal)
			this->inventory.coal++;

		else if (msg->materialsToDeposit.ironOre)
			this->inventory.ironOre++;

		else if (msg->materialsToDeposit.ironIngot)
			this->inventory.ironIngot++;

		else if (msg->materialsToDeposit.sword)
			this->inventory.sword++;
	}

	//Check if there is enough materials to build / upgrade a unit. Returns true if there is enough materials and false if there is not enough materials.
	else if (message->IsA<RequestMaterials>())
	{
		auto msg = message->DynamicCast<RequestMaterials>();
		bool haveEnoughMats = true;

		if (msg->materialsToSend.wood > inventory.wood)
			haveEnoughMats = false;		

		if (msg->materialsToSend.coal > inventory.coal)
			haveEnoughMats = false;

		if (msg->materialsToSend.ironOre > inventory.ironOre)
			haveEnoughMats = false;

		if (msg->materialsToSend.ironIngot > inventory.ironIngot)
			haveEnoughMats = false;

		if (msg->materialsToSend.sword > inventory.sword)
			haveEnoughMats = false;

		if (haveEnoughMats == true)
		{
			inventory.wood -= msg->materialsToSend.wood;
			inventory.coal -= msg->materialsToSend.coal;
			inventory.ironOre -= msg->materialsToSend.ironOre;
			inventory.ironIngot -= msg->materialsToSend.ironIngot;
			inventory.sword -= msg->materialsToSend.sword;
		}

		msg->sender->OnReceiveMessage(RequestMaterialsAnswer::Create(haveEnoughMats));
	}
}

bool InventoryComponent::ReadyToBuild(MaterialsRequired materials)
{
	if (materials.wood > inventory.wood)
		return false;
	if (materials.charcoal > inventory.coal)
		return false;
	if (materials.ironOre > inventory.ironOre)
		return false;
	if (materials.ironIngot > inventory.ironIngot)
		return false;
	if (materials.sword > inventory.sword)
		return false;

	inventory.wood -= materials.wood;
	inventory.coal -= materials.charcoal;
	inventory.ironOre -= materials.ironOre;
	inventory.ironIngot -= materials.ironIngot;
	inventory.sword -= materials.sword;

	return true;
}
