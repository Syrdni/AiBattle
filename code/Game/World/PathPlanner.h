#pragma once
//Components
#include "ECS/BaseComponent.h"
#include "ECS/Entity.h"

//Oryol Containers
#include "Core/Containers/StaticArray.h"

//The level
#include "Game/World/Level.h"

//Used to cancel paths
#include "../Messages/PathCancel.h"
//------------------------------------------------------------------------

// node struct to hold current node id and which node the node came from.
// also contains the g, h, and f cost.
struct node
{
	node() {}
	node(glm::vec3 pos, int came_from, int g, int h) { this->pos = pos; this->came_from = came_from; this->g = g; this->h = h; this->f = g + h; }
	bool operator==(glm::vec3 rhs) { return this->pos == rhs; }
	bool operator==(node rhs) { return this->pos == rhs.pos; }
	glm::vec3 pos;
	int came_from;
	int g;
	int h;
	int f;
};

// request struct to hold progress of a path request, also the return entity.
struct Request
{
	Request(glm::vec3 endPos, Ptr<Entity> entity, node firstNode) { this->endPos = endPos; this->entity = entity; open_list.Add(firstNode); requestDone = false; requestError = false; }
	glm::vec3 endPos;
	Ptr<Entity> entity;

	bool requestDone;
	bool requestError;

	Array<node> open_list;
	Array<node> closed_list;
};

// main component class.
class PathPlanner : public BaseComponent
{
	ECSComponentType(PathPlanner);
public:
	// binds pointer to the level.
	void OnLoad();
	// steps through all activ requests and take one step in the prosses, removes done path and error path.
	void Update();
	// checks if the message is a path request, if so RequestPath();
	void OnReceiveMessage(const Ptr<Message>& message);
	//none.
	void OnDestroy();
	// puts a request in the active requestlist.
	void RequestPath(glm::vec3 endPos, const Ptr<Entity>& entity);

	// sends back either Done or Error message back to the entity.
	void SendDone(Request request);
	void SendError(Request request);
	void SendError(Ptr<Entity> entity);

	// take a step in the path search.
	void Step(Request& currentRequest);

private:
	Array<glm::vec3> GetNeighbours(glm::vec3 pos, const Ptr<Entity>& entity);
	Array<glm::vec3> GetWalkableNeighbours(glm::vec3 pos, const Ptr<Entity>& entity);
	glm::vec3 GetValidNode(glm::vec3 pos, const Ptr<Entity>& entity);
	int CalculateHCost(glm::vec3 evaluatingPos, glm::vec3 endPos);

	unsigned int numSearchCyclesPerUpdate;
	int currentIndex = 0;
	Array<glm::vec3> neighboursIds;
	Array<Request> requests;
	Ptr<Level> level;
};

/// MESSAGES \\\

//------------------------------------------------------------------------

// Message class in case of error.
class PathFailed : public Message
{
	ECSMessageType(PathFailed)
public:
	PathFailed(const String& error) : Error(error) {};
	String Error;
};

// Message class in case of path done.
class PathDone : public Message
{
	ECSMessageType(PathDone)
public:
	PathDone(Array<glm::vec3> path) : Path(path) {};
	Array<glm::vec3> Path;
};

// Message to request a path.
class PathRequest : public Message
{
	ECSMessageType(PathRequest)
public:
	PathRequest(int x, int z, Ptr<Entity> entity) { this->x = x; this->z = z; this->entity = entity; };
	PathRequest(glm::vec3 pos, Ptr<Entity>entity) { this->pos = pos; this->entity = entity; };
	int x;
	int z;
	glm::vec3 pos;
	Ptr<Entity> entity;
};
