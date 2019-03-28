#pragma once
//Message base class
#include "ECS/Message.h"

/**
	Message used to cancel a active path following action.

	@author Philip Stenmark
*/
class PathCancel : public Message
{
	ECSMessageType(PathCancel);

public:
	PathCancel(const Ptr<Entity>& entity) : Entity(entity) {};

	// the entity that is cancelling
	Ptr<Entity> Entity;
};