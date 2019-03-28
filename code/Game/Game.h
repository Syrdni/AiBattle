#pragma once
#include "Core/Main.h"
#include "Gfx/Gfx.h"
#include "IO/IO.h"
#include "IMUI/IMUI.h"
#include "Input/Input.h"
#include "LocalFS/LocalFileSystem.h"
#include "Components/InventoryComponent.h"
#include "PyConsole.h"
#include "ECS/EntityManager.h"

class InventoryComponent;
using namespace Oryol;

class Game : public App
{
public:
	AppState::Code OnInit();
	AppState::Code OnRunning();
	AppState::Code OnCleanup();

	/// the global entity manager
	static EntityManager EntityManager;

	/// contains the game world and its components
	static Ptr<Entity> World;

	/// the python console for executing commands
	static PyConsole Console;

	/// retrieves the current view-projection matrix
	static glm::mat4& GetObserver();



	//Made to check if everything is loaded before trying to get inventorycomponents
	static bool isLoaded;
	static bool isLoaded2;

	static int units[8];


	static bool running;


private:
	/// handles mouse and keyboard inputs
	void HandleInput();

	/// draws user interface
	void DrawUI();

	/// the global camera view
	struct Camera
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 invView;
		glm::mat4 viewProjection;
		glm::vec3 pointOfInterest;
		glm::vec3 eyePos;
		glm::vec2 polar;
		float distance = 20.0f;
		float minLat = 0.0f;
		float maxLat = glm::radians(+85.0f);
		float minDist = 1.0f;
		float maxDist = 100.0f;
		float moveSpeed = 1.0f;
	};

	static Camera camera;


	//Hud position variables
	int oldSize[2] = { 1024, 768 };
	int _width;
	int _height;


	//For keeping track of inventory
	Ptr<InventoryComponent> AIin1;
	Ptr<InventoryComponent> AIin2;
	Inventory AI1inv;
	Inventory AI2inv;
	
	float ResourceHudSize[2] = { 500.0, 85.0 };
	float unitHudSize[2] = { 120.0, 210.0 };
	float rgb_Y[4] = { 1.0, 1.0, 0.0, 1.0 };
	

	bool showConsole = true;
};