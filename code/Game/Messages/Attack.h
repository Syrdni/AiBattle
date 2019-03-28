#pragma once
//Message base class
#include "ECS/Message.h"

class Attack : public Message
{
	ECSMessageType(Attack);
public:
	Attack(int damage) : DamageAmount(damage) {};

	int DamageAmount;
};