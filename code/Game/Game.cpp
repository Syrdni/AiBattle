#pragma once
// Singelton from Json.
#include "DataContainerSingleton.h"
// Components
#include "Components/TeamTag.h"
#include "Components/PathFollower.h"
#include "Components/TransformComponent.h"
#include "Components/ShapeRenderer.h"
#include "Components/LevelVisitor.h"

#include "Components/Explorer.h"
#include "Components/Progress.h"
// World Components
#include "World/Level.h"
#include "World/AICore.h"
#include "World/PathPlanner.h"
#include "World/ShapeServer.h"
#include "World/MessageDispatcher.h"
// Weird stuff.
#include "Game.h"
#include "glm/gtc/random.hpp"
#include "glm/gtx/polar_coordinates.hpp"
#include "glm/gtx/transform.hpp"

EntityManager Game::EntityManager;
Ptr<Entity> Game::World = nullptr;
Game::Camera Game::camera;
PyConsole Game::Console = PyConsole();
bool Game::isLoaded = false;
bool Game::isLoaded2 = false;
bool Game::running = true;
int Game::winner = 0;
//Units[] indexering: 0 = WokerT1, 1 = ExplorerT1, 2 = CraftsmanT1, 3 = SoldiersT1, 4= WorkerT2 etc..
int Game::units[] = { 50,0,0,0, 50,0,0,0, 0,0,0,0, 0,0,0,0 };
OryolMain(Game);

AppState::Code Game::OnInit()
{
	// setup io
	IOSetup ioSetup;
	ioSetup.FileSystems.Add("file", LocalFileSystem::Creator());
	IO::Setup(ioSetup);

	// setup gfx
	auto gfxSetup = GfxSetup::Window(1024, 768, "Lab3");

	gfxSetup.DefaultPassAction = PassAction::Clear(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	gfxSetup.HighDPI = true;
	Gfx::Setup(gfxSetup);

	// setup python and console
	Console.Setup();

	Input::Setup();
	IMUI::Setup();

	// setup world entity
	if (Game::World == nullptr)
	{
		auto& world = Game::EntityManager.CreateEntity();

		IO::LoadGroup(Array<URL>({ "cwd:map2.dat", "cwd:variables.json" }), [world](Array<IO::LoadResult> result)
		{
			// create constants
			DataContainerSingleton::GetInstance().Deserialize((const char*)result[1].Data.Data());

			// add world components
			world->AddComponent<Level>((const char*)result[0].Data.Data());
			world->AddComponent<PathPlanner>();
			world->AddComponent<MessageDispatcher>();
			//world->AddComponent<TransformComponent>()->SetPosition(glm::vec3(-10, 0, -10));

			Game::isLoaded = true;
			Game::isLoaded2 = true;

			AI::Setup();

		}, [](const URL& url, IOStatus::Code status)
		{
			o_info("Failed to load %s\n", url.Path().AsCStr());
		});

		Game::World = world;
		
	}

	// setup camera projection
	int width = Gfx::DisplayAttrs().FramebufferWidth;
	int height = Gfx::DisplayAttrs().FramebufferHeight;
	this->camera.projection = glm::perspectiveFov(glm::radians(70.0f), static_cast<float>(width), static_cast<float>(height), 0.01f, 1000.0f);
	this->camera.polar = glm::vec2(glm::radians(45.0f), glm::radians(45.0f));
	this->camera.pointOfInterest = glm::vec3(0, 0, 0) * Level::TileSize;

	return App::OnInit();
}

AppState::Code Game::OnRunning()
{
	if (AI::doneSetup)
		for (int i = 1; i >= 0; i--)
		{
			AI::players[i].ai->Update();
		}

	this->HandleInput();

	// update camera
	glm::vec3 eucl = glm::euclidean(camera.polar) * camera.distance;
	camera.eyePos = eucl + camera.pointOfInterest;
	camera.view = glm::lookAt(eucl + camera.pointOfInterest, camera.pointOfInterest, glm::vec3(0, 1, 0));
	camera.invView = glm::inverse(camera.view);
	camera.viewProjection = camera.projection * camera.view;

	Gfx::BeginPass();

	// draw ui stuff
	this->DrawUI();

	// update and render entities
	if (running)
		Game::EntityManager.Update();

	ImGui::Render();
	Gfx::EndPass();
	Gfx::CommitFrame();

	return Gfx::QuitRequested() ? AppState::Cleanup : AppState::Running;
}

AppState::Code Game::OnCleanup()
{
	Console.Discard();
	IMUI::Discard();
	Input::Discard();
	Gfx::Discard();
	IO::Discard();
	return App::OnCleanup();
}

glm::mat4& Game::GetObserver()
{
	return camera.viewProjection;
}

void Game::HandleInput()
{
	if (ImGui::IsAnyWindowFocused())
		return;

	if (Input::MouseAttached())
	{
		if (Input::MouseButtonPressed(MouseButton::Left))
		{
			camera.polar.x = glm::clamp(camera.polar.x + Input::MouseMovement().y * 0.01f, camera.minLat, camera.maxLat);
			camera.polar.y -= Input::MouseMovement().x * 0.01f;
		}
		camera.distance = glm::clamp(camera.distance - Input::MouseScroll().y, camera.minDist, camera.maxDist);
	}

	if (Input::KeyboardAttached())
	{
		if (Input::KeyDown(Key::N1))
		{
			camera.distance = 10.0f;
			camera.pointOfInterest = Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team1)->GetAttribute<glm::vec3>("position");
		}

		if (Input::KeyDown(Key::N2))
		{
			camera.distance = 10.0f;
			camera.pointOfInterest = Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team2)->GetAttribute<glm::vec3>("position");
		}

		if (Input::KeyPressed(Key::LeftShift))
			camera.moveSpeed = 1.0f;
		else
			camera.moveSpeed = 0.5f;

		if (Input::KeyPressed(Key::W))
		{
			glm::vec3 offset = -glm::vec3(camera.invView[2][0], camera.invView[2][1], camera.invView[2][2]) * camera.moveSpeed;
			camera.pointOfInterest.x += offset.x;
			camera.pointOfInterest.z += offset.z;
		}

		if (Input::KeyPressed(Key::S))
		{
			glm::vec3 offset = glm::vec3(camera.invView[2][0], camera.invView[2][1], camera.invView[2][2]) * camera.moveSpeed;
			camera.pointOfInterest.x += offset.x;
			camera.pointOfInterest.z += offset.z;
		}

		if (Input::KeyPressed(Key::D))
		{
			glm::vec3 offset = glm::vec3(camera.invView[0][0], camera.invView[0][1], camera.invView[0][2]) * camera.moveSpeed;
			camera.pointOfInterest.x += offset.x;
			camera.pointOfInterest.z += offset.z;
		}

		if (Input::KeyPressed(Key::A))
		{
			glm::vec3 offset = -glm::vec3(camera.invView[0][0], camera.invView[0][1], camera.invView[0][2]) * camera.moveSpeed;
			camera.pointOfInterest.x += offset.x;
			camera.pointOfInterest.z += offset.z;
		}
	}
}

void Game::DrawUI()
{

	
	


	IMUI::NewFrame();
	
	//Skaffar fönstrets höjd och vidd
	_width = Gfx::DisplayAttrs().FramebufferWidth;
	_height = Gfx::DisplayAttrs().FramebufferHeight;
	


	//Debug Window
	//==================================================================================
	ImGui::SetNextWindowPos(ImVec2(4.0f, 4.0f), ImGuiCond_Once);
	ImGui::Begin("Debug", nullptr, ImVec2(200.0f, 200.0f), 0.7f);
	{
		ImGui::Text("Entities: %i / %i", Game::EntityManager.GetNumEntities(), ECSConfig::MaxNumEntities);
		ImGui::Separator();
		ImGui::Checkbox("Show Console", &this->showConsole);
	}
	ImGui::End();

	
	ImGui::SetNextWindowPos(ImVec2(4.0f, 8.0f + 200.0f), ImGuiCond_Once);
	if (this->showConsole)
	{
		Game::Console.Draw();
	}
	

	//Resource HUD window
//==================================================================================
	if (isLoaded)
	{
		if (isLoaded2)
		{
			AIin1 = Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team1)->GetComponent<InventoryComponent>();
			AIin2 = Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team2)->GetComponent<InventoryComponent>();
			AIhp1 = Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team1)->GetComponent<HealthComponent>();
			AIhp2 = Game::World->GetComponent<Level>()->GetCastle(TeamTag::Code::Team2)->GetComponent<HealthComponent>();
			isLoaded2 = false;
		}
		//ImGuiCond_Once	anropa SetNextWindowPos med detta för att det bara ska hända en gång
		ImGui::SetNextWindowPos(ImVec2(00.0f, _height - ResourceHudSize[1]));
		ImGui::Begin("ResourceHUD", nullptr, ImVec2(_width, ResourceHudSize[1]), 0.7f);

		//ImGui::SetWindowSize(ImVec2(_width, ResourceHudSizeY));
		{


			//Get data from components.
			AI1inv = AIin1->getInventory();
			AI2inv = AIin2->getInventory();
			hp1 = AIhp1->getHp();
			hp2 = AIhp2->getHp();

			//Text data for team1
			ImGui::TextColored(ImVec4(rgb_Y[0], rgb_Y[1], rgb_Y[2], rgb_Y[3]), " Team 1		");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), " Health:");
			ImGui::SameLine();
			ImGui::Text("%i		", hp1);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), " Resources:");
			ImGui::SameLine();
			ImGui::Text(" Wood: %i ", AI1inv.wood);
			ImGui::SameLine();
			ImGui::Text(" Coal: %i ", AI1inv.coal);
			ImGui::SameLine();
			ImGui::Text(" IronOre: %i ", AI1inv.ironOre);
			ImGui::SameLine();
			ImGui::Text(" IronIngot: %i ", AI1inv.ironIngot);
			ImGui::SameLine();
			ImGui::Text(" Sword: %i ", AI1inv.sword);
			ImGui::Separator();
			//Text data for team2
			ImGui::TextColored(ImVec4(0.0, 0.74, 1.0, 1.0), " Team 2		");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), " Health:");
			ImGui::SameLine();
			ImGui::Text("%i		", hp2);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), " Resources:");
			ImGui::SameLine();
			ImGui::Text(" Wood: %i ", AI2inv.wood);
			ImGui::SameLine();
			ImGui::Text(" Coal: %i ", AI2inv.coal);
			ImGui::SameLine();
			ImGui::Text(" IronOre: %i ", AI2inv.ironOre);
			ImGui::SameLine();
			ImGui::Text(" IronIngot: %i ", AI2inv.ironIngot);
			ImGui::SameLine();
			ImGui::Text(" Sword: %i ", AI1inv.sword);


		}
		ImGui::End();




		//Unit HUD window
	//============================
	//============================

		ImGui::SetNextWindowPos(ImVec2(0.0f, _height - unitHudSize[1] - ResourceHudSize[1]));
		ImGui::Begin("UnitHUD", nullptr, ImVec2(unitHudSize[0], unitHudSize[1]), 0.7f);
		{

			ImGui::TextColored(ImVec4(rgb_Y[0], rgb_Y[1], rgb_Y[2], rgb_Y[3]), " Team 1");
			ImGui::Text(" Workers:   %i ", units[0]);
			ImGui::Text(" Explorers: %i ", units[1]);
			ImGui::Text(" Craftsman: %i ", units[2]);
			ImGui::Text(" Soldiers:  %i ", units[3]);
			ImGui::Text(" ");
			ImGui::Text(" Kiln:      %i ", units[8]);
			ImGui::Text(" Smelter:   %i ", units[9]);
			ImGui::Text(" Smithy:    %i ", units[10]);
			ImGui::Text(" Barrack:   %i ", units[11]);

			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.0, 0.74, 1.0, 1.0), " Team 2");
			ImGui::Text(" Workers:   %i ", units[4]);
			ImGui::Text(" Explorers: %i ", units[5]);
			ImGui::Text(" Craftsman: %i ", units[6]);
			ImGui::Text(" Soldiers:  %i ", units[7]);
			ImGui::Text(" ");
			ImGui::Text(" Kiln:      %i ", units[12]);
			ImGui::Text(" Smelter:   %i ", units[13]);
			ImGui::Text(" Smithy:    %i ", units[14]);
			ImGui::Text(" Barrack:   %i ", units[15]);

		}
		ImGui::End();

		if (!running)
		{
			ImGui::SetNextWindowPos(ImVec2(500.0f, 500.0f),ImGuiCond_Once);
			ImGui::Begin("Victory Screen!", nullptr, ImVec2(200, 50), 0.7f);
			ImGui::Text("Winner: team %i ", Game::winner);
			ImGui::End();
		}
	}
}




