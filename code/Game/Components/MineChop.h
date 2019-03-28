#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

/*
	Component that worker has when it is cutting down a tree or mining ore
	@author Kristoffer Lundgren, Benjamin Versterlund
*/
class MineChop : public BaseComponent
{
	ECSComponentType(MineChop);
};

