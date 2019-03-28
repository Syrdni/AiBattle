#pragma once
//Message base class
#include "ECS/Message.h"

class Abort : public Message
{
	ECSMessageType(Abort);
public:
	Abort(const Ptr<Entity>& entity) : Entity(entity) {};

	Ptr<Entity> Entity;
};