#pragma once
//Message base class
#include "ECS/Message.h"

/**
	Message sent when an entity has finished its path.

	@author Philip Stenmark
*/
class PathArrive : public Message
{
	ECSMessageType(PathArrive);
};