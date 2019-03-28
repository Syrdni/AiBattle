#pragma once
//Message base class
#include "ECS/Message.h"
/**
	Message sent when an entity can attack again, sent from attacker to attacker with a delay.

	@author Benjamin Vesterlund, Sandra Andersson, Simon Rogowski
*/
class AttackCooldown : public Message
{
	ECSMessageType(AttackCooldown);
};