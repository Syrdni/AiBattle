#include "PathPlanner.h"
#include "../Messages/Abort.h"

void PathPlanner::OnLoad()
{
	this->level = entity->GetComponent<Level>();
	this->numSearchCyclesPerUpdate = 25;
	this->neighboursIds = {
	glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, -1.0),	// right, bottom right
	glm::vec3(0.0, 0.0, -1.0), glm::vec3(-1.0, 0.0, -1.0),	// bottom, bottom left
	glm::vec3(-1.0, 0.0, 0.0), glm::vec3(-1.0, 0.0, 1.0),	// left, top left
	glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 1.0) };	// top, top right
}

void PathPlanner::Update()
{
	if (this->requests.Empty())
		return;

	for (size_t i = 0; i < this->numSearchCyclesPerUpdate; i++)
	{
		Request &currentRequst = requests[currentIndex];
		Step(currentRequst);
		if (currentRequst.requestDone)
		{
			SendDone(currentRequst);
			requests.Erase(currentIndex);
		}
		else if (currentRequst.requestError)
		{
			SendError(currentRequst);
			requests.Erase(currentIndex);
		}

		if (currentIndex == 0)
		{
			if (this->requests.Empty())
				return;
			currentIndex = requests.Size() - 1;
		}
		else
			currentIndex--;
	}

}

void PathPlanner::OnReceiveMessage(const Ptr<Message>& message)
{
	if (message->IsA<PathRequest>())
	{
		auto msg = message->DynamicCast<PathRequest>();
		glm::vec3 pos = msg->pos;
		if (!level->GetTile(pos)->CanPass(msg->entity))
		{
			pos = GetValidNode(pos / level->TileSize, msg->entity);
		}
		RequestPath(pos / level->TileSize, msg->entity);
	}
	if (message->IsA<Abort>())
	{
		auto msg = message->DynamicCast<Abort>();

		for (int i = 0; i < requests.Size(); i++)
		{
			if (requests[i].entity->Id == msg->Entity->Id)
			{
				requests.Erase(i);
			}
		}
	}
}

glm::vec3 PathPlanner::GetValidNode(glm::vec3 pos, const Ptr<Entity>& entity)
{
	// Searches for availible node nearby.
	int searchDiameter = 3;
	glm::vec3 p1 = glm::vec3(pos.x, pos.y, pos.z);
	glm::vec3 p2 = glm::vec3(pos.x, pos.y, pos.z);

	// Search in a square-like pattern starting from the corners, going inwards
	while (true) {
		if (p1.x > 0 && p1.z > 0)
		{
			p1 -= glm::vec3(1, 0, 1);

			if (p1.x + searchDiameter - 1 > 99)
			{
				p1.x = 99 - searchDiameter + 1;
			}
			if (p1.z + searchDiameter - 1 > 99)
			{
				p1.z = 99 - searchDiameter + 1;
			}		
		}
		if (p2.x < 99 && p2.z < 99)
		{
			p2 += glm::vec3(1, 0, 1);
			if (p2.x - searchDiameter + 1 < 0)
			{
				p2.x = searchDiameter - 1;
			}
			if (p2.z - searchDiameter + 1 < 0)
			{
				p2.z = searchDiameter - 1;
			}	
		}

		for (int i = 0; i < searchDiameter; i++) {
			if (level->GetTile(p1.x + i, p1.z)->CanPass(entity))
				return glm::vec3(p1.x + i, p1.y, p1.z) * level->TileSize;
			else if (level->GetTile(p1.x, p1.z + i)->CanPass(entity))
				return glm::vec3(p1.x, p1.y, p1.z + i) * level->TileSize;
			else if (level->GetTile(p2.x - i, p2.z)->CanPass(entity))
				return glm::vec3(p2.x - i, p2.y, p2.z) * level->TileSize;
			else if (level->GetTile(p2.x, p2.z - i)->CanPass(entity))
				return glm::vec3(p2.x, p2.y, p2.z - i) * level->TileSize;

		}

		searchDiameter += 2;
	}
}

void PathPlanner::OnDestroy()
{
}

void PathPlanner::RequestPath(glm::vec3 endPos, const Ptr<Entity>& entity)
{
	glm::vec3 entityPos = entity->GetAttribute<glm::vec3>("position") / level->TileSize;
	Request temp(endPos, entity, node(entityPos, -1, 0, 0));
	requests.Add(temp);
}

void PathPlanner::Step(Request& currentRequest)
{
	if (currentRequest.open_list.Empty())
	{
		currentRequest.requestError = true;
		return;
	}

	// Get node with best f-value from openlist
	node current_Node = currentRequest.open_list[0];
	int currentNodeIndex = 0;
	for (int i = 1; i < currentRequest.open_list.Size() - 1; i++)
	{
		if (currentRequest.open_list[i].f < current_Node.f)
		{
			current_Node = currentRequest.open_list[i];
			currentNodeIndex = i;
		}
	}

	currentRequest.closed_list.Add(current_Node);

	current_Node = currentRequest.closed_list.Back();

	currentRequest.open_list.Erase(currentNodeIndex);

	if (!currentRequest.open_list.Empty())
	{
		currentRequest.open_list.Trim();
	}


	if (current_Node.pos.x == currentRequest.endPos.x && current_Node.pos.z == currentRequest.endPos.z)
	{
		currentRequest.requestDone = true;
		return;
	}

	Array<glm::vec3> neighbourPositions = GetWalkableNeighbours(current_Node.pos, currentRequest.entity);
	int i = -1;
	for (glm::vec3 &neighbourPos : neighbourPositions)
	{
		i++;

		int length;
		// Check if diagonal
		if (glm::abs(current_Node.pos.x - neighbourPos.x) + glm::abs(current_Node.pos.z - neighbourPos.z) == 1)
		{
			length = 10;
		}
		else
		{
			length = 14;
		}

		auto endNode = currentRequest.endPos;
		auto tile = level->GetTile(neighbourPos.x, neighbourPos.z);
		auto node_ = node(neighbourPos, currentRequest.closed_list.Size() - 1, current_Node.g + length * 1 / tile->GetSpeedMod(), CalculateHCost(neighbourPos, currentRequest.endPos));

		//Check if neighbour is in the open list
		bool neighbourInOpenList = false;

		for (int i = 0; i <= currentRequest.open_list.Size() - 1; i++)
		{

			if (currentRequest.open_list[i] == neighbourPos)
			{
				neighbourInOpenList = true;
				// If neighbor in openList can be accessed quicker through currentNode. Set currentNode as parent and update g.
				if (currentRequest.open_list[i].g > node_.g)
				{
					currentRequest.open_list[i].g = node_.g;
					currentRequest.open_list[i].came_from = node_.came_from;
				}

				break;
			}
		}

		if (neighbourInOpenList)
			continue;

		//check if neighbour is in the closed list.
		bool neighbourInClosedList = false;
		for (int i = 0; i < currentRequest.closed_list.Size() - 1; i++)
		{
			node closedNode = currentRequest.closed_list[i];
			if (closedNode == neighbourPos)
			{
				neighbourInClosedList = true;

				break;
			}
		}

		if (neighbourInClosedList)
			continue;

		currentRequest.open_list.Add(node_);

	}
}

int PathPlanner::CalculateHCost(glm::vec3 evaluatingPos, glm::vec3 endPos)
{
	int x = abs(evaluatingPos[0] - endPos[0]) * 10;
	int y = abs(evaluatingPos[2] - endPos[2]) * 10;
	return abs(evaluatingPos[0] - endPos[0]) * 10 + abs(evaluatingPos[2] - endPos[2]) * 10;
}

Array<glm::vec3> PathPlanner::GetNeighbours(glm::vec3 pos, const Ptr<Entity>& entity)
{
	Array<glm::vec3> result;
	for (auto& offset : neighboursIds)
	{
		glm::vec3 vec = (offset + pos);
		if (vec.x < 0 || vec.x > 99 || vec.z < 0 || vec.z > 99)
			continue;

		result.Add(vec);
	}
	return result;
}

Array<glm::vec3> PathPlanner::GetWalkableNeighbours(glm::vec3 pos, const Ptr<Entity>& entity)
{
	Array<glm::vec3> result;
	bool right, bottom, left, top = false;

	// Check if each neighbour is on walkable tile and add them. For diagonals, corner cutting is not allowed
	if (level->GetTile(neighboursIds[0].x + pos.x, neighboursIds[0].z + pos.z)->CanPass(entity)) // Right
	{
		right = true;
		result.Add(neighboursIds[0] + pos);
	}
	if (level->GetTile(neighboursIds[2].x + pos.x, neighboursIds[2].z + pos.z)->CanPass(entity)) // Bottom
	{
		bottom = true;
		result.Add(neighboursIds[2] + pos);
	}
	if (level->GetTile(neighboursIds[4].x + pos.x, neighboursIds[4].z + pos.z)->CanPass(entity)) // Left
	{
		left = true;
		result.Add(neighboursIds[4] + pos);
	}
	if (level->GetTile(neighboursIds[6].x + pos.x, neighboursIds[6].z + pos.z)->CanPass(entity)) // Top
	{
		top = true;
		result.Add(neighboursIds[6] + pos);
	}
	if (level->GetTile(neighboursIds[1].x + pos.x, neighboursIds[1].z + pos.z)->CanPass(entity) && bottom && right) // Bottom Right
	{
		result.Add(neighboursIds[1] + pos);
	}
	if (level->GetTile(neighboursIds[3].x + pos.x, neighboursIds[3].z + pos.z)->CanPass(entity) && bottom && left) // Bottom Left
	{
		result.Add(neighboursIds[3] + pos);
	}
	if (level->GetTile(neighboursIds[5].x + pos.x, neighboursIds[5].z + pos.z)->CanPass(entity) && top && left) // Top Left
	{
		result.Add(neighboursIds[5] + pos);
	}
	if (level->GetTile(neighboursIds[7].x + pos.x, neighboursIds[7].z + pos.z)->CanPass(entity) && top && right) // Top Right
	{
		result.Add(neighboursIds[7] + pos);
	}

	return result;
}

void PathPlanner::SendDone(Request request)
{
	node current = request.closed_list.Back();
	Array<glm::vec3> path;
	float tileSize = level->TileSize;
	// Build path in reverse
	while (current.came_from != -1)
	{
		path.Insert(0, glm::round(current.pos * tileSize));
		current = request.closed_list[current.came_from];
	}

	request.entity->OnReceiveMessage(PathDone::Create(path));
}

void PathPlanner::SendError(Request request)
{
	request.entity->OnReceiveMessage(PathFailed::Create("Path not found"));
}

void PathPlanner::SendError(Ptr<Entity> entity)
{
	entity->OnReceiveMessage(PathFailed::Create("Invalid end_node"));
}
