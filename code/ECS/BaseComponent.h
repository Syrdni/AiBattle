#pragma once
#include "ECSUtil.h"
#include "Message.h"
#include "Attribute.h"

using namespace Oryol;

class BaseComponent : public RefCounted
{
	OryolBaseTypeDecl(BaseComponent);
public:
	friend class Entity;

	/// called when this component is added to an entity
	virtual void OnLoad() {};

	/// updates this component
	virtual void Update() {};

	/// updates this component at a fixed time step
	virtual void FixedUpdate() {};

	/// called when a message was received
	virtual void OnReceiveMessage(const Ptr<Message>& message) {};

	/// called when this component is destroyed
	virtual void OnDestroy() {};

protected:
	/// the owner of this component
	Entity* entity;
};
