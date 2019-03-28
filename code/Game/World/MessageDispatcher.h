#pragma once
//Base classes
#include "ECS/BaseComponent.h"
#include "ECS/Message.h"
#include "ECS/Entity.h"
#include "ECS/EntityManager.h"

struct MessageContainer
{
	Ptr<Message> message;
	Ptr<Entity> receiver;
	int timeToSend;
};

class MessageDispatcher : public BaseComponent 
{
	ECSComponentType(MessageDispatcher);
public:
	void OnLoad();
	void FixedUpdate();
	void DispatchMessage(const Ptr<Entity>& entity, const Ptr<Message>& message, int delay);
private:
	Array<MessageContainer> pending;
	int localTick;
};