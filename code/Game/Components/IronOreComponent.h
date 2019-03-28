#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//glm
#include "glm/vec3.hpp"
#include "glm/gtc/random.hpp"

//Components
#include "TransformComponent.h"
#include "Game/Components/Progress.h"

//Used to render a model
#include "ShapeRenderer.h"

//Components
#include "LevelVisitor.h"

//Used to get the model for the entity
#include "../World/ShapeServer.h"

//Singleton for the JSON file
#include "Game/DataContainerSingleton.h"

class Progress;

class IronOreComponent : public BaseComponent
{
	ECSComponentType(IronOreComponent)
public:
	IronOreComponent(glm::vec3 vector);
	void OnLoad();
private:
	glm::vec3 position;
};

inline IronOreComponent::IronOreComponent(glm::vec3 vector) {
	this->position = vector;
}

inline void IronOreComponent::OnLoad() {
	auto transform = entity->AddComponent<TransformComponent>();
	transform->SetPosition(position);
	transform->Rotate(glm::linearRand(0, 360), glm::vec3(0, 1, 0));
	entity->AddComponent<ShapeRenderer>(ShapeServer::GetIronOreModel());
	entity->AddComponent<LevelVisitor>();
	entity->AddComponent<Progress>(DataContainerSingleton::GetInstance().GetMaterialMap()["ironOre"].timeToCreate);

}
