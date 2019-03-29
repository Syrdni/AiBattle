#pragma once
// Singelton of Json file
#include "Game/DataContainerSingleton.h"

// Component checks
#include "ECS/Entity.h"
#include "ECS/BaseComponent.h"

// Messages
#include "Game/Messages/ProgressMsg.h"
#include "Game/Messages/Attack.h"
#include "Game/Messages/AttackCooldown.h"

// Components

// type components
#include "Game/Components/Worker.h"
#include "Game/Components/Explorer.h"
#include "Game/Components/TreeComponent.h"
#include "Game/Components/IronOreComponent.h"
#include "Game/Components/BuildingSiteComponent.h"
#include "Game/Components/TrainingCampComponent.h"
#include "Game/Components/SmithyComponent.h"
#include "Game/Components/SmelterComponent.h"
#include "Game/Components/KilnComponent.h"

// running components
#include "Game/Components/ShapeRenderer.h"
#include "Game/Components/TransformComponent.h"
#include "Game/Components/PathFollower.h"
#include "Game/Components/LevelVisitor.h"
#include "Game/Components/TeamTag.h"
#include "Game/Components/InventoryComponent.h"
#include "Game/Components/TaskPlanner.h"
#include "Game/Components/Progress.h"
#include "Game/Components/Discoverable.h"

// World Components
#include "Game/World/PathPlanner.h"
#include "Game/World/ShapeServer.h"
#include "Game/World/MessageDispatcher.h"

// Weird Stuff
#include "Game/Game.h"
#include "Modules/Core/String/String.h"
#include "Core/Containers/StaticArray.h"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "pybind11/stl.h"
#include <map>

namespace py = pybind11;

#define invalidId 0xFFFFFFFFFFFFFFFF
#define AI_FOUND_CAUSE_CREATED "created"

namespace AI
{
	struct PosStruct
	{
		float* worldX;
		float* worldZ;
	};

	class AICore
	{
	public:
		/// Setup Variables, create workers and start Python AI
		AICore(int team);
		/// Calls update on Python AI
		void Update();
		/// Handles messages and sends PythonMessages to Python 
		void OnReceiveMessage(const Ptr<Message>& message);
		/// Tells Python it has found an enity (if it is not konwn already) 
		void foundEntity(Oryol::Ptr<Entity> entity, char* cause);
		/// Tells Python it has lost track of an enity (if it is not unkonwn already) 
		void lostEntity(Oryol::Ptr<Entity> entity, char* cause);
		/// Aborts all tasks on unit
		void AbortTask(uint64_t unitID);
		/// Tells unit to move to tile x,z
		void MoveTo(uint64_t unitID, int x, int z);
		/// Given an entity, a type and/or an upgrade site this function can upgrade buildings and units
		void Upgrade(uint64_t unitID, int entType, uint64_t upgradeSite);

		void Attack(uint64_t attackerId, uint64_t victimId);

		/// Tells unit to move to the building and then starts crafting an item
		void Craft(uint64_t unitID, uint64_t buildingID);

		/// Tells unit to move to, harvest and pickup harvest and deliver it to castle
		void HarvestResource(uint64_t unitID, uint64_t harvestID);
		/// Spawns a worksite if there is enough recoures in castle, also removes recources, returns if spawed
		void Deliver(uint64_t unitID, uint64_t pickupID);



		void Drop(uint64_t unitID, uint64_t dropID);
		/// Given a position and a building type the function spawn a building site
		bool SpawnWorksite(int type, int x, int z);
		/// Tells a builder to move to and build a building
		void Build(uint64_t builder, uint64_t bulding);
		/// This function return a string corresponding to the provided integer.
		Oryol::String getTypeString(WorkerTypes integer);
		/// Gets reference to units position 
		PosStruct GetPos(uint64_t UnitID);

	private:
		/// this AI:s team
		int team = TeamTag::Code::Team2;
		/// what the startfile of python is called
		std::string pythonHead = "main";

		Ptr<InventoryComponent> inventory_team_1;
		Ptr<InventoryComponent> inventory_team_2;
		DataContainerSingleton & instance = DataContainerSingleton::GetInstance();

		/// The string to execute to update AI
		Oryol::String PY_update;
		/// The string to execute for AI to receve message
		Oryol::String PY_onMessage;
		
		friend class Game;
		/// Map of ids known to AI, converts numbers to Oryol::Id
		Oryol::Map<uint64_t, Oryol::Id> entities;
	};

	struct AIStruct {
		/// Pointer to AI core
		AICore* ai;
		/// Message to be recived by python
		std::map<char*, char*> msg;
	};

	/// The Ai plyers AICore adds iteslf to this in init
	static AIStruct players[2];

	/// A message to send to Python
	class PythonMessage : public Message
	{
		ECSMessageType(PythonMessage)
	public:
		std::map<char*, char*> map;
	};

	/// Expose all the functions that are used to control the entitis to python so that they can be controlled by the AI
	PYBIND11_EMBEDDED_MODULE(AI, m)
	{
		py::class_<AICore>(m, "AICore")
			.def("MoveTo", &AICore::MoveTo)
			.def("Abort", &AICore::AbortTask)
			.def("HarvestResource", &AICore::HarvestResource)
			.def("Deliver", &AICore::Deliver)
			.def("Drop", &AICore::Drop)
			.def("SpawnWorksite", &AICore::SpawnWorksite)
			.def("Build", &AICore::Build)
			.def("Upgrade", &AICore::Upgrade)
			.def("Attack", &AICore::Attack)
			.def("GetPos", &AICore::GetPos)
			.def("Craft", &AICore::Craft)
			.attr("tileSize") = Level::TileSize;

		py::class_<AIStruct>(m, "AIStruct")
			.def_readonly("ai", &AIStruct::ai)
			.def_readonly("msg", &AIStruct::msg);

		py::class_<PosStruct>(m, "posStruct")
			.def("__init__", [](PosStruct){})
			.def_readonly("worldX", &PosStruct::worldX)
			.def_readonly("worldZ", &PosStruct::worldZ);

		m.attr("player1") = &players[0];
		m.attr("player2") = &players[1];
	}

	/// Creates Worker
	Oryol::Ptr<Entity> createWorker(int Team)
	{
		auto& unit = Game::EntityManager.CreateEntity();

		auto transform = unit->AddComponent<TransformComponent>();
		transform->SetPosition(Game::World->GetComponent<Level>()->GetCastle(Team)->GetAttribute<glm::vec3>("position"));
		
		unit->AddComponent<Worker>();
		unit->AddComponent<TeamTag>(Team == 1 ? TeamTag::Code::Team1 : TeamTag::Code::Team2);
		unit->AddComponent<ShapeRenderer>(ShapeServer::GetWorkerModel(Team == 1 ? TeamTag::Code::Team1 : TeamTag::Code::Team2));
		unit->AddComponent<PathFollower>();
		unit->AddComponent<LevelVisitor>();
		unit->AddComponent<TaskPlanner>();
		unit->AddComponent<HealthComponent>(10);
		unit->AddComponent<Discoverable>(Discoverable::Unit);

		return unit;
	}

	inline AICore::AICore(int team) : team(team)
	{
		entities = Oryol::Map<uint64_t, Oryol::Id>();

		players[team - 1].ai = this;

		py::exec("import sys");
		py::exec("sys.path.append('" + std::string(AI_URL) + "/AI" + (team == 1 ? "1" : "2") + "')");

		pythonHead += team == 1 ? "1" : "2";
		Game::Console.Execute(Oryol::String(("import " + pythonHead).c_str()), true);
		this->PY_update = Oryol::String((pythonHead + ".update()").c_str());
		this->PY_onMessage = Oryol::String((pythonHead + ".onMessage()").c_str());

		Game::Console.Execute(Oryol::String((pythonHead + ".startup()").c_str()),true);

		foundEntity(Game::World->GetComponent<Level>()->GetCastle(team), AI_FOUND_CAUSE_CREATED);

		for (size_t i = 0; i < 50; i++)
		{
			auto unit = AI::createWorker(team);
			//auto unit = Game::EntityManager.GetEntity(entities[id]);
			foundEntity(unit, AI_FOUND_CAUSE_CREATED);
		}
 

		inventory_team_1 = Game::World->GetComponent<Level>()->GetCastle(1)->GetComponent<InventoryComponent>();
		inventory_team_2 = Game::World->GetComponent<Level>()->GetCastle(2)->GetComponent<InventoryComponent>();
	}
 

	/// Sets entityType of PythonMessage 
	void setEntityType(Oryol::Ptr<Entity> entity, Oryol::Ptr<PythonMessage> pm)
	{
		if (entity->HasComponent<Discoverable>())
		{
			// checks type of entity
			switch (entity->GetComponent<Discoverable>()->Type)
			{
			case Discoverable::Tree:
				pm->map["entityType"] = "tree";
				break;
			case Discoverable::IronOre:
				pm->map["entityType"] = "ironOre";
				break;
			case Discoverable::Castle:
				pm->map["entityType"] = "castle";
				break;
			case Discoverable::Unit: 
				if (entity->HasComponent<Worker>())
				{
					pm->map["entityType"] = "worker";
				}
				else if (entity->HasComponent<Explorer>())
				{
					pm->map["entityType"] = "explorer";
				}
				else if (entity->HasComponent<Craftsman>())
				{
					pm->map["entityType"] = "craftsman";
				}
				else if (entity->HasComponent<Soldier>())
				{
					pm->map["entityType"] = "soldier";
				}
				break;
			case Discoverable::Building:
				if (entity->HasComponent<KilnComponent>())
				{
					pm->map["entityType"] = "kiln";
				}
				else if (entity->HasComponent<BuildingSiteComponent>())
				{
					pm->map["entityType"] = "buildingSite";
				}
				else if (entity->HasComponent<SmithyComponent>())
				{
					pm->map["entityType"] = "smithy";
				}
				else if (entity->HasComponent<TrainingCampComponent>())
				{
					pm->map["entityType"] = "trainingCamp";
				}
				else if (entity->HasComponent<SmelterComponent>())
				{
					pm->map["entityType"] = "smelter";
				}
				break;
			default:
				pm->map["entityType"] = "UNKNOWN";
				break;
			}
		}
	}

	AI::PosStruct AICore::GetPos(uint64_t ID)
	{
		if (!entities.Contains(ID))
		{
			Game::Console.Log("Attempt to acces invalid unit id: %i", ID);
			return PosStruct();
		}

		glm::vec3* trans = Game::EntityManager.GetEntity(entities[ID])->GetComponent<TransformComponent>()->GetPositionReference();
		auto Pstruct = PosStruct();

		Pstruct.worldX = &trans->x;
		Pstruct.worldZ = &trans->z;
		return Pstruct;
	}

	void AICore::foundEntity(Oryol::Ptr<Entity> entity, char* cause)
	{
		if ( ! entities.AddUnique(entity->Id.Value, entity->Id))
		{
			//retun if entity already known 
			return;
		}

		auto pm = PythonMessage::Create();
		pm->map["type"] = "entityFound";

		if (entity->HasComponent<TeamTag>())
		{
			pm->map["team"] = entity->GetAttribute<int>("team") == 1 ? "1" : "2";
		}
		else
		{
			pm->map["team"] = "0";
		}
		
		pm->map["cause"] = cause;

		setEntityType(entity, pm);

		auto idStr = std::to_string(entity->Id.Value);
		pm->map["ID"] = const_cast<char*>(idStr.c_str());
	
		auto pos = entity->GetAttribute<glm::vec3>("position");
		auto posXStr = std::to_string(pos[0]);
		pm->map["posx"] = const_cast<char*>(posXStr.c_str());
		auto posZStr = std::to_string(pos[2]);
		pm->map["posz"] = const_cast<char*>(posZStr.c_str());
		
		

		this->OnReceiveMessage(pm);
	}

	inline void AI::AICore::lostEntity(Oryol::Ptr<Entity> entity, char * cause)
	{
		//check if item in list 
		if (!entities.Contains(entity->Id.Value))
		{
			return;
		}

		auto pm = PythonMessage::Create();
		pm->map["type"] = "entityLost";

		if (entity->HasComponent<TeamTag>())
		{
			pm->map["team"] = entity->GetAttribute<int>("team") == 1 ? "1" : "2";
		}
		else
		{
			pm->map["team"] = "0";
		}
		pm->map["cause"] = cause;
		setEntityType(entity, pm);

		auto idStr = std::to_string(entity->Id.Value);
		pm->map["ID"] = const_cast<char*>(idStr.c_str());

		this->OnReceiveMessage(pm);
	}

	inline void AICore::Update()
	{
		Game::Console.Execute(PY_update, true);
	}

	inline void AICore::OnReceiveMessage(const Ptr<Message>& message)
	{
		if (message->IsA<PythonMessage>())
		{
			players[team - 1].msg = message->DynamicCast<PythonMessage>()->map;
			Game::Console.Execute(PY_onMessage, true);
		}
	}

	inline void AICore::AbortTask(uint64_t unitID)
	{
		if (entities.Contains(unitID))
		{
			Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TaskPlanner>()->ClearTasks();
		}
		else
		{
			Game::Console.Log("Attempt to acces invalid unit id: %i", unitID);
		}
	}

	inline void AICore::MoveTo(uint64_t unitID, int x, int z)
	{
		if (entities.Contains(unitID))
		{
			auto ent = Game::EntityManager.GetEntity(entities[unitID]);
			auto taskPlanner = ent->GetComponent<TaskPlanner>();
			auto entityPos = Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TransformComponent>();
			MoveTask* task = new MoveTask(x, z);
			taskPlanner->AddTask(task);
		}
		else
		{
			Game::Console.Log("Attempt to access invalid unit id: %i", unitID);
		}
	}

	inline void AICore::HarvestResource(uint64_t unitID, uint64_t harvestID)
	{
		if (entities.Contains(unitID))
		{
			// Move to resource
			auto taskPlanner = Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TaskPlanner>();
			auto entityPos = Game::EntityManager.GetEntity(entities[harvestID])->GetComponent<TransformComponent>()->GetPosition();

			MoveTask * moveTask;

			if (Game::EntityManager.GetEntity(entities[harvestID])->GetComponent<Discoverable>()->Type == Discoverable::Tree)
				moveTask = new MoveTask(glm::round(entityPos.x / Level::TileSize), glm::round(entityPos.z / Level::TileSize));
			else
				moveTask = new MoveTask(entityPos.x / Level::TileSize, entityPos.z / Level::TileSize);

			taskPlanner->AddTask(moveTask);
			
			// Harvest resource
			HarvestTask* harvestTask = new HarvestTask(entities[unitID], entities[harvestID]);

			taskPlanner->AddTask(harvestTask);
			
			// Move to castle
			Oryol::Ptr<Entity> castle = Game::World->GetComponent<Level>()->GetCastle(this->team);
			glm::vec3 castlePos = castle->GetComponent<TransformComponent>()->GetPosition();
			moveTask = new MoveTask(castlePos.x / Level::TileSize, castlePos.z / Level::TileSize);

			taskPlanner->AddTask(moveTask);

			// Drop off resource
			DropOffTask* dropOffTask = new DropOffTask(castle, entities[harvestID]);
			taskPlanner->AddTask(dropOffTask);
		}
		else
		{
			Game::Console.Log("Attempt to acces invalid unit id: %i", unitID);
		}
	}

	inline void AICore::Deliver(uint64_t unitID, uint64_t pickupID)
	{
		if (entities.Contains(unitID))
		{
			// Move to resource
			auto pickupEntity = Game::EntityManager.GetEntity(entities[pickupID]);
			auto taskPlanner = Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TaskPlanner>();
			auto pickupPos = Game::EntityManager.GetEntity(entities[pickupID])->GetComponent<TransformComponent>()->GetPosition();
			MoveTask* moveTask = new MoveTask(pickupPos.x / Level::TileSize, pickupPos.z / Level::TileSize);

			taskPlanner->AddTask(moveTask);

			// Pick up resource
			PickupTask* pickupTask = new PickupTask(entities[pickupID]);

			taskPlanner->AddTask(pickupTask);

			// Move to castle
			Oryol::Ptr<Entity> castle = Game::World->GetComponent<Level>()->GetCastle(this->team);
			glm::vec3 castlePos = castle->GetComponent<TransformComponent>()->GetPosition();
			moveTask = new MoveTask(castlePos.x / Level::TileSize, castlePos.z / Level::TileSize);

			taskPlanner->AddTask(moveTask);

			// Drop off resource
			DropOffTask* dropOffTask = new DropOffTask(castle, entities[pickupID]);
			taskPlanner->AddTask(dropOffTask);
		}
		else
		{
			Game::Console.Log("Attempt to acces invalid unit id: %i", pickupID);
		}
	}

	inline void AICore::Drop(uint64_t unitID, uint64_t dropID)
	{
		if (entities.Contains(unitID))
		{
			auto taskPlanner = Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TaskPlanner>();
			DropTask* dropTask = new DropTask(entities[dropID]);
			taskPlanner->AddTask(dropTask);

		}
		else
			Game::Console.Log("Attempt to access invalid unit id: %i", unitID);
	}

	inline void AICore::Upgrade(uint64_t unitID, int entType, uint64_t upgradeSite = invalidId)
	{
		if (upgradeSite == invalidId)
		{
			if (entities.Contains(unitID))
			{
				if (Game::EntityManager.GetEntity(entities[unitID])->GetAttribute<int>("team") == 1)
					inventory_team_1->ReadyToBuild(instance.GetUnitMap()[getTypeString(WorkerTypes(entType))].materialsRequired);
				else
					inventory_team_2->ReadyToBuild(instance.GetUnitMap()[getTypeString(WorkerTypes(entType))].materialsRequired);

				Game::EntityManager.GetEntity(entities[unitID])->AddComponent<Progress>(instance.GetUnitMap()[getTypeString(WorkerTypes(entType))].upgradeTime);
				Game::EntityManager.GetEntity(entities[unitID])->AddComponent<Upgrading>();
				auto taskPlanner = Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TaskPlanner>();
				taskPlanner->ClearTasks();
				UpgradeTask* task = new UpgradeTask();
				task->upgradeType = entType;
				taskPlanner->AddTask(task);
				taskPlanner->NextTask();
			}
			else
				Game::Console.Log("Attemt to access invalid unit id: %i", unitID);
		}
		else
			if (entities.Contains(unitID))
			{
				glm::vec3 entityPos = Game::EntityManager.GetEntity(entities[unitID])->GetAttribute<glm::vec3>("position");
				bool atCamp = false;
				auto list = Game::World->GetComponent<Level>()->GetEntities(entityPos);
				for (auto entity : list)
					if (entity->HasComponent<TrainingCampComponent>())
						atCamp = true;
				if (atCamp)
				{
					if (Game::EntityManager.GetEntity(entities[unitID])->GetAttribute<int>("team") == 1)
						inventory_team_1->ReadyToBuild(instance.GetUnitMap()[getTypeString(WorkerTypes(entType))].materialsRequired);
					else
						inventory_team_2->ReadyToBuild(instance.GetUnitMap()[getTypeString(WorkerTypes(entType))].materialsRequired);

					Game::EntityManager.GetEntity(entities[unitID])->AddComponent<Progress>();						// added but unsure where it should be.. like lookwise
					auto taskPlanner = Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TaskPlanner>();
					taskPlanner->ClearTasks();
					UpgradeTask* task = new UpgradeTask();
					task->timeRequired = instance.GetUnitMap()[getTypeString(WorkerTypes(entType))].upgradeTime;	// workertypes(entType) known weak point... maybe send int instead!
					task->upgradeType = entType;
					taskPlanner->AddTask(task);
					taskPlanner->NextTask();
				}
			}
	}

	inline void AICore::Attack(uint64_t attackerId, uint64_t victimId)
	{
		if (!Game::EntityManager.IsValid(entities[victimId])) {
			auto message = AI::PythonMessage::Create();
			message->map["type"] = "attackFailed";

			auto idStr = std::to_string(entities[attackerId].Value);
			message->map["ID"] = const_cast<char*>(idStr.c_str());
			OnReceiveMessage(message);
			return;
		}

		Ptr<Entity> attacker = Game::EntityManager.GetEntity(entities[attackerId]);
		Ptr<Entity> victim = Game::EntityManager.GetEntity(entities[victimId]);

		auto taskPlanner = Game::EntityManager.GetEntity(entities[attackerId])->GetComponent<TaskPlanner>();
		taskPlanner->ClearTasks();
		
		auto pos = victim->GetComponent<TransformComponent>()->GetPosition();
		MoveTask* task = new MoveTask(pos.x / Level::TileSize, pos.z / Level::TileSize);
		taskPlanner->AddTask(task);

		AttackTask* attackTask = new AttackTask(entities[attackerId], entities[victimId]);

		taskPlanner->AddTask(attackTask);

		////TODO Fix better attack check with position instead of while loop.
		//if (attacker->HasComponent<Soldier>() && attacker->GetComponent<Soldier>()->canAttack)
		//{
		//	auto list = Game::World->GetComponent<Level>()->GetEntities(attacker->GetAttribute<glm::vec3>("position"));
		//	for (auto entity : list)
		//		if (entity == victim)
		//		{
		//			victim->OnReceiveMessage(Attack::Create(5));				// damage set by Krig on random.. (blame him, not Boris) 21/3 -19
		//			Game::World->GetComponent<MessageDispatcher>()->DispatchMessage(attacker, AttackCooldown::Create(), 2); // random delay.. blame krig.. . not Boris.
		//			attacker->GetComponent<Soldier>()->canAttack = false;
		//		}
		//}
		//else
		//	Game::Console.Log("Entity %i can not attack", attacker->Id.SlotIndex);

	}

	inline void AICore::Craft(uint64_t unitID, uint64_t buildingID)
	{
		Ptr<Entity> craftsman = Game::EntityManager.GetEntity(entities[unitID]);
		Ptr<Entity> building = Game::EntityManager.GetEntity(entities[buildingID]);

		auto taskPlanner = Game::EntityManager.GetEntity(entities[unitID])->GetComponent<TaskPlanner>();
		taskPlanner->ClearTasks();

		auto pos = building->GetComponent<TransformComponent>()->GetPosition();
		if(pos != craftsman->GetComponent<TransformComponent>()->GetPosition())
		{
			MoveTask* task = new MoveTask(pos.x / Level::TileSize, pos.z / Level::TileSize);
			taskPlanner->AddTask(task);
		}
		
		CraftTask* craftTask = new CraftTask(entities[unitID], entities[buildingID]);

		taskPlanner->AddTask(craftTask);
	}

	inline bool AICore::SpawnWorksite(int type, int x, int z)
	{
		if (
			Game::World->GetComponent<Level>()->GetTile(x, z)->IsBuildable() && 
			Game::World->GetComponent<Level>()->GetEntities<BuildingSiteComponent>(glm::vec3(x * Level::TileSize, 0, z * Level::TileSize)).Size() == 0 &&
			Game::World->GetComponent<Level>()->GetEntities<KilnComponent>(glm::vec3(x * Level::TileSize, 0, z * Level::TileSize)).Size() == 0 &&
			Game::World->GetComponent<Level>()->GetEntities<SmelterComponent>(glm::vec3(x * Level::TileSize, 0, z * Level::TileSize)).Size() == 0 &&
			Game::World->GetComponent<Level>()->GetEntities<SmithyComponent>(glm::vec3(x * Level::TileSize, 0, z * Level::TileSize)).Size() == 0 &&
			Game::World->GetComponent<Level>()->GetEntities<TrainingCampComponent>(glm::vec3(x * Level::TileSize, 0, z * Level::TileSize)).Size() == 0
		)
		{
			auto buildMap = DataContainerSingleton::GetInstance().GetBuildingMap();
			BuildingInfo uppgradesTo;
			switch (type)
			{
			case BuildingTypes::kiln:
				uppgradesTo = buildMap["charcoalPile"];
				break;
			case BuildingTypes::smithy:
				uppgradesTo = buildMap["smithy"];
				break;
			case BuildingTypes::smelter:
				uppgradesTo = buildMap["smelter"];
				break;
			case BuildingTypes::trainingCamp:
				uppgradesTo = buildMap["trainingCamp"];
				break;
			default:
				return false;
			}

			if (Game::World->GetComponent<Level>()->GetCastle(this->team)->GetComponent<InventoryComponent>()->ReadyToBuild(uppgradesTo.materialsRequired))
			{
				auto building = Game::EntityManager.CreateEntity();
				building->AddComponent<Discoverable>(Discoverable::Building);
				building->AddComponent<TeamTag>(this->team);
				building->AddComponent<HealthComponent>(buildMap["buildingSite"].health);
				building->AddComponent<Progress>(uppgradesTo.buildtime);
				building->AddComponent<BuildingSiteComponent>(type);
				auto transform8 = building->AddComponent<TransformComponent>();
				transform8->SetPosition(glm::vec3(Level::TileSize * x, Level::TileSize, Level::TileSize * z));
				building->AddComponent<ShapeRenderer>(ShapeServer::GetBuildingSiteModel());
				building->AddComponent<LevelVisitor>();

				foundEntity(building, AI_FOUND_CAUSE_CREATED);

				return true;
			}
		}
		return false;
	}

	inline void AI::AICore::Build(uint64_t builder, uint64_t building)
	{
		auto buildingEnt = Game::EntityManager.GetEntity(entities[building]);

		auto taskPlanner = Game::EntityManager.GetEntity(entities[builder])->GetComponent<TaskPlanner>();
	
		auto pos = buildingEnt->GetAttribute<glm::vec3>("position");
		MoveTask* task = new MoveTask(pos.x / Level::TileSize, pos.z / Level::TileSize);
		taskPlanner->AddTask(task);
		
		BuildTask* BT = new BuildTask(buildingEnt);
		taskPlanner->AddTask(BT);
	}

	inline Oryol::String AICore::getTypeString(WorkerTypes integer)
	{
		Oryol::String type;
		switch (integer) {
		case 1: type = "explorer";
			break;
		case 2: type = "craftsman";
			break;
		case 3: type = "soldier";
			break;
		}
		return type;
	}

	//boll to track if AI has bean set up
	bool doneSetup = false;

	inline void Setup()
	{
		new AICore(2);
		new AICore(1);
		doneSetup = true;
	}

	//calls lostEntity on both AIs
	inline void lostEntity(Oryol::Ptr<Entity> entity, char * cause)
	{
		for (auto player : players)
		{
			player.ai->lostEntity(entity, cause);
		}
	}
}


//stuff to avoid link errors

inline void TaskPlanner::SendMessage(PyMessage msg) {
	auto message = AI::PythonMessage::Create();
	message->map = msg;
	AI::players[entity->GetAttribute<int>("team") - 1].ai->OnReceiveMessage(message);
}

inline void TaskPlanner::LoseResource(Oryol::Ptr<Entity> entity, char * cause)
{
	AI::lostEntity(entity, cause);
}

void Level::OnReceiveMessage(const Ptr<Message>& message)
{
	if (message->IsA<TileChange>())
	{
		auto msg = message->DynamicCast<TileChange>();
		auto& seeker = msg->Entity;

		// change entity tile offset
		this->entityMap[msg->LastTile].Erase(seeker);
		this->entityMap[msg->NextTile].Add(seeker);

		if (seeker->HasComponent<TeamTag>())
		{
			int myteam = seeker->GetAttribute<int>("team");
			if (msg->NextTile->visibleBits == ANY_VISIBLE && msg->LastTile->visibleBits != ANY_VISIBLE)
			{
				// visible to enemies
				AI::players[myteam & 1].ai->foundEntity(seeker, "entered FoW");
			}

			if (msg->NextTile->visibleBits != ANY_VISIBLE && msg->LastTile->visibleBits == ANY_VISIBLE)
			{
				// no longer visible to enemies
				AI::players[myteam & 1].ai->lostEntity(seeker, "exited FoW");
			}
		}

		if (seeker->HasComponent<Explorer>())
		{
			int index = msg->NextTile->x * width + msg->NextTile->z;
			int team = seeker->GetComponent<TeamTag>()->Team;

			auto tile = this->GetTile(index);

			for (auto& offset : this->tileOffsets)
			{
				auto tile = this->GetTile(index + offset);

				// Castle discover problem fix
				if (tile->x == 4 && tile->z == 11)
					AI::players[team - 1].ai->foundEntity(Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team1), "discovered");
				else if (tile->x == 95 && tile->z == 88)
					AI::players[team - 1].ai->foundEntity(Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team2), "discovered");

				for (auto& e : this->entityMap[tile])
				{
					if (e->HasComponent<Discoverable>() && e != seeker)
					{
						AI::players[team - 1].ai->foundEntity(e, "discovered");
					}
				}
				tile->visibleBits |= team;
			}
		}
	}

	if (message->IsA<EntityRemove>())
	{
		auto msg = message->DynamicCast<EntityRemove>();
		this->entityMap[msg->Tile].Erase(msg->Entity);
		
	}
}

inline void Soldier::SendMessage(PyMessage msg) {
	auto message = AI::PythonMessage::Create();
	message->map = msg;
	AI::players[entity->GetAttribute<int>("team") - 1].ai->OnReceiveMessage(message);
}