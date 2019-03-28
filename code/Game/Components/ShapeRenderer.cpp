#include "ShapeRenderer.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Graphics stuff
#include "Assets/Gfx/ShapeBuilder.h"
#include "glm/gtx/transform.hpp"
#include "../World/ShapeServer.h"

//Components
#include "LevelVisitor.h"
#include "TransformComponent.h"

//Other
#include "../World/Level.h"
#include "Game/Game.h"

ShapeRenderer::ShapeRenderer(DrawState drawState)
{
	this->drawState = drawState;
}

void ShapeRenderer::OnLoad()
{
	entity->RegisterAttribute("visible", &this->visible, Attribute::ReadWrite);
	transform = entity->GetComponent<TransformComponent>();
}

void ShapeRenderer::Update()
{
	if (this->visible)
	{
		this->shaderParams.mvp = Game::GetObserver() * transform->GetWorldTransform();
		this->shaderParams.model = transform->GetWorldTransform();

		Gfx::ApplyDrawState(this->drawState);
		Gfx::ApplyUniformBlock(this->shaderParams);
		Gfx::Draw();
	}
}