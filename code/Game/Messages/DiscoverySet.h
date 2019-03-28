#pragma once
//Message base class
#include "ECS/Message.h"

//Entity Class
#include "ECS/Entity.h"

/**
	Message used when an explorer discovers a set of resources or level objects.
	Note that a discovery set may contain some identical resources as a preivously sent set.

	@author Philip Stenmark
*/
class DiscoverySet : public Message
{
	ECSMessageType(DiscoverySet);
public:
	DiscoverySet(const Set<Ptr<Entity>>& set) : Discoveries(set) {};

	// only contains entities with a discoverable component attached
	Set<Ptr<Entity>> Discoveries;
};