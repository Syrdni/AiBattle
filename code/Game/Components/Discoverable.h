#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

/**
	Include this component for entities that may be discoverable by explorers.
	The type of discovery is provided in the constructor.

	@author Philip Stenmark
*/
class Discoverable : public BaseComponent
{
	ECSComponentType(Discoverable);
public:
	enum Type
	{
		Tree,
		IronOre,
		Castle,
		Unit,
		Building,
	};

	Discoverable(Discoverable::Type type) : Type(type) {};

	// the type of discovery
	int Type;
};