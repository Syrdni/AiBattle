#pragma once
//Message base class
#include "ECS/Message.h"

/*
	Message that is sent when a worker has been upgraded
	@author Kristoffer Lundgre
*/
class UpgradeComplete : public Message
{
	ECSMessageType(UpgradeComplete);
};