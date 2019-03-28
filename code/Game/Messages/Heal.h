#pragma once
//Message base class
#include "ECS/Message.h"

class Heal : public Message
{
	ECSMessageType(Heal);
public:
	Heal(int amount) : HealAmount(amount) {};

	int HealAmount;
};