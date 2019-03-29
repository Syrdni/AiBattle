#pragma once
// Component checks
#include "ECS/BaseComponent.h"
#include "ECS/Entity.h"
// Messages
#include "Game/Messages/PathArrive.h"
#include "Game/Messages/Abort.h"
#include "Game/Messages/PathCancel.h"
#include "Game/Messages/UpgradeComplete.h"
#include "Game/Messages/ProgressMsg.h"
#include "Game/Messages/Attack.h"
#include "Game/Messages/AttackCooldown.h"
// Components
#include "Game/Components/BuildBuilding.h"
#include "Game/Components/MineChop.h"
#include "Game/Components/BuildingSiteComponent.h"
#include "Game/Components/IronOreComponent.h"
#include "Game/Components/InventoryComponent.h"
#include "Game/Components/Discoverable.h"
#include "Game/Components/Craftsman.h"
// Weird Components
#include "Game/Game.h"
#include "Game/DataContainerSingleton.h"
#include "Game/World/PathPlanner.h"
#include <map>
#include "Game/World/MessageDispatcher.h"

typedef std::map<char*, char*> PyMessage;

class Craftsman;

struct Task {
	enum TaskType {
		MoveTask,
		UpgradeTask,
		HarvestTask,
		DropOffTask,
		BuildTask,
		AttackTask,
		PickupTask,
		DropTask,
		CraftTask
	};

	TaskType type;
};

// Task-specific structs

struct MoveTask : public Task {
	MoveTask(int x, int z) : x(x), z(z) { type = TaskType::MoveTask; };
	int x;
	int z;
};

struct UpgradeTask : public Task {
	UpgradeTask() { type = TaskType::UpgradeTask; };
	int timeRequired;
	int upgradeType;
};

struct HarvestTask : public Task {
	HarvestTask(Id unit, Id target) : unit(unit), target(target) { type = TaskType::HarvestTask; };
	Id unit;
	Id target;
};

struct DropOffTask : public Task {
	DropOffTask(Oryol::Ptr<Entity> castle, Id resource) : castle(castle), resource(resource) { type = TaskType::DropOffTask; };
	Oryol::Ptr<Entity> castle;
	Id resource;
};

struct PickupTask : public Task {
	PickupTask(Id resource) : resource(resource) { type = TaskType::PickupTask; };
	Id resource;
};

struct DropTask : public Task {
	DropTask(Id resource) : resource(resource) { type = TaskType::DropTask; };
	Id resource;
};

struct BuildTask : public Task
{
	BuildTask(Oryol::Ptr<Entity> building) : building(building) { type = TaskType::BuildTask; };
	Oryol::Ptr<Entity> building;
};

struct AttackTask : public Task {
	AttackTask(Id attacker, Id victim) : attacker(attacker), victim(victim) { type = TaskType::AttackTask; };
	Id attacker;
	Id victim;
};

struct CraftTask : public Task {
	CraftTask(Id craftsman, Id building) : craftsman(craftsman), building(building) { type = TaskType::CraftTask; };
	Id craftsman;
	Id building;

};

// Class used to queue tasks and execute them
class TaskPlanner : public BaseComponent
{
	ECSComponentType(TaskPlanner);
public:
	void AddTask(Task* task);
	void ClearTasks();
	void OnReceiveMessage(const Ptr<Message>& message);
	void FixedUpdate();
	void NextTask();
	void RemoveTask();
private:
	Array<Task*> tasks;
	DataContainerSingleton  &data = DataContainerSingleton::GetInstance();
	void SendMessage(PyMessage msg);
	void LoseResource(Oryol::Ptr<Entity> entity, char * cause);
	// bool to decide whether to run next task during update.
	bool runNext = false;
};

// Add new task to queue
inline void TaskPlanner::AddTask(Task* task) {
	if (tasks.Size() == 0)
		runNext = true;
	tasks.Add(task);
}

// Add new task to queue
inline void TaskPlanner::ClearTasks() {
	
	// Get first task in queue
	if (tasks.Size() == 0)
		return;
	Task* task = tasks[0];

	// Don't allow 
	if (task->type == Task::TaskType::UpgradeTask)
	{
		PyMessage msg;
		msg["type"] = "abortFailed";
		auto idStr = std::to_string(entity->Id.Value);
		msg["ID"] = const_cast<char*>(idStr.c_str());
		SendMessage(msg);
		return;
	}

	// If-statements to send specific Abort-messages to other entities for certain tasks

	if (task->type == Task::TaskType::HarvestTask)
	{
		if (Game::EntityManager.IsValid(((HarvestTask*)task)->target)) {
			Oryol::Ptr<Entity> targetEntity = Game::EntityManager.GetEntity(((HarvestTask*)task)->target);
			targetEntity->OnReceiveMessage(Abort::Create(entity));
		}
	}
	else if (task->type == Task::TaskType::BuildTask)
	{
		Oryol::Ptr<Entity> targetEntity = ((BuildTask*)task)->building;
		targetEntity->OnReceiveMessage(Abort::Create(entity));
	}
	else if (task->type == Task::TaskType::MoveTask) {
		Game::World->OnReceiveMessage(PathCancel::Create(entity));
	}

	// Send abort message to all components in entity
	entity->OnReceiveMessage(Abort::Create(entity));
	
	// Delete pointer instances before clearing list
	for (auto& task : tasks)
	{
		delete task;
	}
	tasks.Clear();

	runNext = false;
}

// Runs next task if previous task is done
inline void TaskPlanner::FixedUpdate()
{
	if (runNext)
		this->NextTask();
}

// Runs the next task in the queue
inline void TaskPlanner::NextTask() {
	if (tasks.Size() > 0) {
		Task* task = tasks[0];

		runNext = false;

		// Run first task in queue

		if (task->type == Task::TaskType::MoveTask) {
			MoveTask* moveTask = (MoveTask*)task;
			Game::World->OnReceiveMessage(PathRequest::Create(glm::vec3(moveTask->x * Level::TileSize, Level::TileSize, moveTask->z * Level::TileSize), entity));
		}
		else if (task->type == Task::TaskType::UpgradeTask)
		{
			UpgradeTask* upgradeTask = (UpgradeTask*)task;
			entity->OnReceiveMessage(StartProgress::Create(entity, upgradeTask->upgradeType));
		}
		else if (task->type == Task::TaskType::HarvestTask) {
			HarvestTask* harvestTask = (HarvestTask*)task;

			if (Game::EntityManager.IsValid(harvestTask->target) && Game::EntityManager.GetEntity(harvestTask->target)->HasComponent<LevelVisitor>()) {
				entity->AddComponent<MineChop>();
				Game::EntityManager.GetEntity(harvestTask->target)->OnReceiveMessage(StartProgress::Create(entity, 0));
			}
			else {
				PyMessage msg;
				msg["type"] = "harvestFailed";
				auto idStr = std::to_string(entity->Id.Value);
				msg["ID"] = const_cast<char*>(idStr.c_str());
				this->ClearTasks();
				SendMessage(msg);
			}
		}
		else if (task->type == Task::TaskType::BuildTask) 
		{
			BuildTask* buildTask = (BuildTask*)task;
			entity->AddComponent<BuildBuilding>();
			buildTask->building->OnReceiveMessage(StartProgress::Create(entity, buildTask->building->GetComponent<BuildingSiteComponent>()->buildingType));
		}
		else if (task->type == Task::TaskType::PickupTask)
		{
			PickupTask* pickupTask = (PickupTask*)task;

			auto pickupTransform = Game::EntityManager.GetEntity(pickupTask->resource)->GetComponent<TransformComponent>();

			pickupTransform->SetPosition(glm::vec3(0.05f, 0.12f, -0.4));

			if (Game::EntityManager.GetEntity(pickupTask->resource)->HasComponent<IronOreComponent>()) // If entity is iron ore
			{
				pickupTransform->Scale(glm::vec3(-0.4f, -0.4f, -0.4f));
			}

			pickupTransform->SetRotation(glm::vec3(0, 0, 0));
			pickupTransform->Rotate(90.0f, glm::vec3(0, 0, 1));
			pickupTransform->AttachTo(entity);

			Game::EntityManager.GetEntity(pickupTask->resource)->RemoveComponent<LevelVisitor>();

			this->RemoveTask();

			PyMessage msg;
			msg["type"] = "pickupComplete";
			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());
			SendMessage(msg);
		}
		else if (task->type == Task::TaskType::DropOffTask)
		{
			DropOffTask* dropOffTask = (DropOffTask*)task;
			MaterialsToDeposit materials = MaterialsToDeposit();
			auto resource = Game::EntityManager.GetEntity(dropOffTask->resource);

			o_assert_dbg(resource->HasComponent<Discoverable>());
			switch (resource->GetComponent<Discoverable>()->Type)
			{
			case Discoverable::Tree:
				materials.wood = true;
				break;
			case Discoverable::IronOre:
				materials.ironOre = true;
				break;
			}

			dropOffTask->castle->GetComponent<InventoryComponent>()->OnReceiveMessage(Deposit::Create(materials));
			TransformComponent tempTransform;
			Game::EntityManager.GetEntity(dropOffTask->resource)->GetComponent<TransformComponent>()->Detach();
			Game::EntityManager.RemoveEntity(dropOffTask->resource);

			LoseResource(resource, "harvested");
			// TODO: Call ResourceLost when implemented.

			this->RemoveTask();

			PyMessage msg;
			msg["type"] = "dropOffComplete";
			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());
			if (materials.wood) {
				msg["resourceType"] = "tree";
			}
			else {
				msg["resourceType"] = "ironOre";
			}
			SendMessage(msg);
		}
		else if (task->type == Task::TaskType::DropTask)
		{
			DropTask* dropTask = (DropTask*)task;

			auto dropTransform = Game::EntityManager.GetEntity(dropTask->resource)->GetComponent<TransformComponent>();

			dropTransform->Detach();

			dropTransform->SetPosition(entity->GetComponent<TransformComponent>()->GetPosition());

			Game::EntityManager.GetEntity(dropTask->resource)->AddComponent<LevelVisitor>();

			if (Game::EntityManager.GetEntity(dropTask->resource)->HasComponent<IronOreComponent>()) // If entity is iron ore
			{
				dropTransform->Scale(glm::vec3(0.4f, 0.4f, 0.4f));
			}

			this->RemoveTask();

			PyMessage msg;
			msg["type"] = "dropComplete";
			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());
			SendMessage(msg);
		}
		else if (task->type == Task::TaskType::AttackTask)
		{
			AttackTask* attackTask = (AttackTask*)task;
			auto attacker = Game::EntityManager.GetEntity(attackTask->attacker);
			Ptr<Entity> victim;

			if (Game::EntityManager.IsValid(attackTask->victim)) {
				victim = Game::EntityManager.GetEntity(attackTask->victim);
			}
			else {
				PyMessage msg;
				msg["type"] = "attackFailed";
				auto idStr = std::to_string(entity->Id.Value);
				msg["ID"] = const_cast<char*>(idStr.c_str());
				SendMessage(msg);

				RemoveTask();
				return;
			}

			if (attacker->HasComponent<Soldier>() && attacker->GetComponent<Soldier>()->canAttack)
			{
				auto list = Game::World->GetComponent<Level>()->GetEntities(attacker->GetAttribute<glm::vec3>("position")); // TODO: change loop into single comparison between attacker and victim
				
				// Attack Castle
				if (victim->HasComponent<InventoryComponent>()) {
					victim->OnReceiveMessage(Attack::Create(data.GetAttackDamage()));
					Game::World->GetComponent<MessageDispatcher>()->DispatchMessage(attacker, AttackCooldown::Create(), data.GetAttackCooldown());
					attacker->GetComponent<Soldier>()->canAttack = false;
				}
				// Attack entities
				else {
					for (auto entity : list)
						if (entity == victim)
						{
							victim->OnReceiveMessage(Attack::Create(data.GetAttackDamage()));
							Game::World->GetComponent<MessageDispatcher>()->DispatchMessage(attacker, AttackCooldown::Create(), data.GetAttackCooldown());
							attacker->GetComponent<Soldier>()->canAttack = false;
							break;
						}
				}

				PyMessage msg;
				msg["type"] = "attackSuccessful";
				auto idStr = std::to_string(entity->Id.Value);
				msg["ID"] = const_cast<char*>(idStr.c_str());
				SendMessage(msg);

				RemoveTask();
			}
			else
			{
				PyMessage msg;
				msg["type"] = "attackFailed";
				auto idStr = std::to_string(entity->Id.Value);
				msg["ID"] = const_cast<char*>(idStr.c_str());
				SendMessage(msg);

				RemoveTask();
			}
		}
		else if (task->type == Task::TaskType::CraftTask)
		{
			CraftTask* craftTask = (CraftTask*)task;
			auto craftsman = Game::EntityManager.GetEntity(craftTask->craftsman);
			auto building = Game::EntityManager.GetEntity(craftTask->building);
			auto inventory = Game::World->GetComponent<Level>()->GetCastle(craftsman->GetAttribute<int>("team"))->GetComponent<InventoryComponent>();

			if(building->HasComponent<KilnComponent>())
			{
				inventory->ReadyToBuild(data.GetMaterialMap()["charcoal"].materialsRequired);
				building->OnReceiveMessage(StartProgress::Create(craftsman, 0));
			}
			else if(building->HasComponent<SmelterComponent>())
			{
				inventory->ReadyToBuild(data.GetMaterialMap()["ironIngot"].materialsRequired);
				building->OnReceiveMessage(StartProgress::Create(craftsman, 0));
			}
			else if(building->HasComponent<SmithyComponent>())
			{
				inventory->ReadyToBuild(data.GetMaterialMap()["sword"].materialsRequired);
				building->OnReceiveMessage(StartProgress::Create(craftsman, 0));
			}

		}
		runNext = false;
	}
	else {
		// If there are no more tasks, send message telling AI there are no tasks left
		PyMessage msg;
		msg["type"] = "tasksComplete";
		auto idStr = std::to_string(entity->Id.Value);
		msg["ID"] = const_cast<char*>(idStr.c_str());
		runNext = false;
		SendMessage(msg);
	}
}

// Remove first task in queue
inline void TaskPlanner::RemoveTask() {
	delete tasks.PopFront();
	runNext = true;
	if (tasks.Size() == 0)
	{
		PyMessage msg;
		msg["type"] = "tasksComplete";
		auto idStr = std::to_string(entity->Id.Value);
		msg["ID"] = const_cast<char*>(idStr.c_str());
		runNext = false;
		SendMessage(msg);
	}
}

// Method to recieve messages from other components
inline void TaskPlanner::OnReceiveMessage(const Ptr<Message>& message) {
	if (tasks.Size() == 0)
		return;

	if (message->IsA<PathArrive>())
	{
		if (tasks[0]->type == Task::TaskType::MoveTask)
		{
			PyMessage msg;
			msg["type"] = "moveComplete";
			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());

			this->RemoveTask();
			SendMessage(msg);
		}
	}
	else if (message->IsA<ProgressDone>())
	{



		if (tasks[0]->type == Task::TaskType::HarvestTask)
		{
			auto harvestMsg = message->DynamicCast<ProgressDone>();
			auto harvestTransform = harvestMsg->entity->GetComponent<TransformComponent>();
			auto entityPos = entity->GetComponent<TransformComponent>()->GetPosition();
			auto harvestPos = harvestTransform->GetPosition();

			PyMessage msg;

			if (glm::round(entityPos.x / Level::TileSize) == glm::round(harvestPos.x / Level::TileSize) && glm::round(entityPos.z / Level::TileSize) == glm::round(harvestPos.z / Level::TileSize))
			{
				msg["type"] = "harvestComplete";
				auto idStr = std::to_string(entity->Id.Value);
				msg["ID"] = const_cast<char*>(idStr.c_str());

				harvestTransform->SetPosition(glm::vec3(0.05f, 0.12f, -0.4));

				if (harvestMsg->entity->GetComponent<Discoverable>()->Type == Discoverable::IronOre) // If entity is iron ore
				{
					harvestTransform->Scale(glm::vec3(-0.4f, -0.4f, -0.4f));
				}

				harvestTransform->SetRotation(glm::vec3(0, 0, 0));
				harvestTransform->Rotate(90.0f, glm::vec3(0, 0, 1));
				harvestTransform->AttachTo(entity);
				harvestMsg->entity->RemoveComponent<LevelVisitor>();
				this->RemoveTask();
				SendMessage(msg);
			}
			else
			{
				msg["type"] = "harvestFailed";
				auto idStr = std::to_string(entity->Id.Value);
				msg["ID"] = const_cast<char*>(idStr.c_str());
				this->ClearTasks();
				SendMessage(msg);
			}
			entity->RemoveComponent<MineChop>();
		}
		else if (tasks[0]->type == Task::TaskType::BuildTask)
		{
			PyMessage msg;
			msg["type"] = "buildComplete";
			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());

			this->RemoveTask();
			SendMessage(msg);
		}
		else if (tasks[0]->type == Task::TaskType::CraftTask)
		{
			PyMessage msg;
			msg["type"] = "craftComplete";
			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());

			this->RemoveTask();
			SendMessage(msg);
		}

		
	}
	else if(message->IsA<UpgradeComplete>())
	{
		if (tasks[0]->type == Task::TaskType::UpgradeTask)
		{
			PyMessage msg;
			msg["type"] = "upgradeComplete";
			auto idStr = std::to_string(entity->Id.Value);
			msg["ID"] = const_cast<char*>(idStr.c_str());

			this->RemoveTask();
			SendMessage(msg);
		}
	}
}