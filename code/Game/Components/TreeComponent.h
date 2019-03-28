#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Glm
#include "glm/vec3.hpp"
#include "glm/gtc/random.hpp"

//Components
#include "TransformComponent.h"
#include "LevelVisitor.h"
#include "Game/Components/Progress.h"

//Singleton for the JSON file
#include "Game/DataContainerSingleton.h"

//Used to render a model
#include "ShapeRenderer.h"

//Used to get the model for the entity
#include "../World/ShapeServer.h"


class TreeComponent : public BaseComponent
{
	ECSComponentType(TreeComponent)
public:
	TreeComponent(glm::vec3 vector);
	void OnLoad();
private:
	glm::vec3 position;
};

inline TreeComponent::TreeComponent(glm::vec3 vector) {
	this->position = vector;
}

inline void TreeComponent::OnLoad() {
	auto transform = entity->AddComponent<TransformComponent>();
	transform->SetPosition(this->position);
	transform->Rotate(glm::linearRand(0, 360), glm::vec3(0, 1, 0));
	entity->AddComponent<ShapeRenderer>(ShapeServer::GetTreeModel());
	entity->AddComponent<LevelVisitor>();
	entity->AddComponent<Progress>(DataContainerSingleton::GetInstance().GetMaterialMap()["wood"].timeToCreate);
}