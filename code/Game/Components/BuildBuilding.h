#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"
/*
	Component is added to builder when it is building a buildiung.
	@author Kristoffer Lundgren, Benjamin Vesterlund
*/
class BuildBuilding : public BaseComponent
{
	ECSComponentType(BuildBuilding);
};