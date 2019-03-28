#include "TransformComponent.h"

void TransformComponent::OnLoad()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::quat(1, 0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	transformDirtyFlag = true;

	entity->RegisterAttribute("worldTransform", &worldTransform, Attribute::Read);
	entity->RegisterAttribute("localTransform", &localTransform, Attribute::Read);
	entity->RegisterAttribute("transformChanged", &transformDirtyFlag, Attribute::ReadWrite);

	entity->RegisterAttribute("position", &position, Attribute::Read);
	entity->RegisterAttribute("rotation", &rotation, Attribute::Read);
	entity->RegisterAttribute("scale", &scale, Attribute::Read);
}

void TransformComponent::Update()
{
	if (this->transformDirtyFlag)
	{
		glm::mat4 t = glm::translate(this->position);
		glm::mat4 r = glm::toMat4(this->rotation);
		glm::mat4 s = glm::scale(this->scale);

		localTransform = t * r * s;
		transformDirtyFlag = false;
	}

	if (parent != nullptr)
	{
		worldTransform = parent->GetWorldTransform() * localTransform;
	}
	else
	{
		worldTransform = localTransform;
	}
}

void TransformComponent::OnDestroy()
{
}

void TransformComponent::AttachTo(const Ptr<Entity>& parent)
{
	o_assert(parent->HasComponent<TransformComponent>());
	this->parent = parent->GetComponent<TransformComponent>();;
}

void TransformComponent::Detach()
{
	this->parent = nullptr;
}

bool TransformComponent::IsTransformDirty()
{
	return this->transformDirtyFlag;
}

void TransformComponent::SetPosition(const glm::vec3 & position)
{
	this->position = position;
	this->transformDirtyFlag = true;
}

void TransformComponent::SetRotation(const glm::vec3 & eulers)
{
	this->rotation = glm::quat(1, 0, 0, 0);
	Rotate(eulers.z, glm::vec3(0, 0, 1));
	Rotate(eulers.y, glm::vec3(0, 1, 0));
	Rotate(eulers.x, glm::vec3(1, 0, 0));
	this->transformDirtyFlag = true;
}

void TransformComponent::SetScale(const glm::vec3 & scale)
{
	this->scale = scale;
	this->transformDirtyFlag = true;
}

void TransformComponent::SetDirection(const glm::vec3 & direction)
{
	o_assert_dbg(direction != glm::vec3(0, 0, 0));

	glm::vec3 tmp = -direction;
	tmp = glm::normalize(tmp);
	
	if (tmp == glm::vec3(0, 1, 0))
	{
		this->SetRotation(glm::vec3(-90.0f, 0, 0));
	}
	else
	{
		glm::vec3 xv = glm::cross(glm::vec3(0, 1, 0), tmp);
		xv = glm::normalize(xv);

		glm::vec3 yv = glm::cross(tmp, xv);
		yv = glm::normalize(yv);

		this->rotation = glm::quat_cast(glm::mat3(xv, yv, tmp));
	}
	this->transformDirtyFlag = true;
}

void TransformComponent::Translate(const glm::vec3 & offset)
{
	this->position += offset;
	this->transformDirtyFlag = true;
}

void TransformComponent::Rotate(float degrees, const glm::vec3 & axis)
{
	this->rotation = glm::rotate(this->rotation, glm::radians(degrees), axis);
	this->transformDirtyFlag = true;
}

void TransformComponent::Scale(const glm::vec3 & offset)
{
	this->scale += offset;
	this->transformDirtyFlag = true;
}

glm::mat4 TransformComponent::GetLocalTransform() const
{
	return this->localTransform;
}

glm::mat4 TransformComponent::GetWorldTransform() const
{
	return this->worldTransform;
}

glm::vec3 TransformComponent::GetPosition() const
{
	return this->position;
}

glm::quat TransformComponent::GetRotation() const
{
	return this->rotation;
}

glm::vec3 TransformComponent::GetScale() const
{
	return this->scale;
}

glm::vec3 TransformComponent::GetForward() const
{
	return this->rotation * glm::vec3(0, 0, -1);
}

glm::vec3 TransformComponent::GetUp() const
{
	return this->rotation * glm::vec3(0, 1, 0);
}

glm::vec3 TransformComponent::GetRight() const
{
	return this->rotation * glm::vec3(1, 0, 0);
}

glm::vec3 TransformComponent::GetEulerAngles() const
{
	auto& e = glm::eulerAngles(this->rotation);
	return glm::degrees(e);
}

glm::vec3 * TransformComponent::GetPositionReference()
{
	return &position;
}
