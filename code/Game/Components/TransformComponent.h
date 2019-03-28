#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

//Glm
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

class TransformComponent : public BaseComponent
{
	ECSComponentType(TransformComponent);
public:
	void OnLoad();

	void Update();

	void OnDestroy();

	/// attach this transform to a parent
	void AttachTo(const Ptr<Entity>& parent);

	/// detach transform from any parent
	void Detach();

	/// checks if the transform has changed
	bool IsTransformDirty();

	/// sets the position
	void SetPosition(const glm::vec3& position);

	/// sets the rotation
	void SetRotation(const glm::vec3& eulers);

	/// sets the scale
	void SetScale(const glm::vec3& scale);

	/// sets the direction the forward vector of this transform should point at
	void SetDirection(const glm::vec3& direction);

	/// translates this transform by given offset
	void Translate(const glm::vec3& offset);

	/// rotates this transform by given degrees around axis
	void Rotate(float degrees, const glm::vec3& axis);

	/// scales this transform by given offset
	void Scale(const glm::vec3& offset);

	/// gets the local transform matrix
	glm::mat4 GetLocalTransform() const;

	/// gets the world transform matrix
	glm::mat4 GetWorldTransform() const;

	/// gets the position
	glm::vec3 GetPosition() const;

	/// gets the rotation
	glm::quat GetRotation() const;

	/// gets the scale
	glm::vec3 GetScale() const;

	/// gets the forward vector
	glm::vec3 GetForward() const;

	/// gets the up vector
	glm::vec3 GetUp() const;

	/// gets the right vector
	glm::vec3 GetRight() const;

	/// gets current rotation in euler angles
	glm::vec3 GetEulerAngles() const;

	glm::vec3* GetPositionReference();

private:
	Ptr<TransformComponent> parent;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	glm::mat4 worldTransform;
	glm::mat4 localTransform;
	bool transformDirtyFlag;
};