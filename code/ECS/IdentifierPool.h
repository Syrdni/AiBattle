#pragma once
#include "Core/Containers/Queue.h"
#include "Resource/Id.h"

using namespace Oryol;

/*
	A lightweight identifier pool implementation based on Oryol's ResourcePool
*/
class IdentifierPool
{
public:
	// setup the identifier pool
	void Setup(int resourceType, int poolSize);

	// allocate a new identifier
	Id Allocate();

	// deallocate an identifier
	void Deallocate(const Id& id);

private:
	uint32 uniqueCounter;
	uint16 resourceType;
	Queue<uint16> freeSlots;
	bool isValid;
};

inline void IdentifierPool::Setup(int resourceType, int poolSize)
{
	this->freeSlots.SetFixedCapacity(poolSize);

	for (uint16 i = 0; i < poolSize; i++)
		this->freeSlots.Enqueue(i);

	this->resourceType = resourceType;
	this->uniqueCounter = 0;
	this->isValid = true;
}

inline Id IdentifierPool::Allocate()
{
	o_assert_dbg(this->isValid);
	o_assert_dbg(this->resourceType != Id::InvalidType);
	return Id(this->uniqueCounter++, this->freeSlots.Dequeue(), this->resourceType);
}

inline void IdentifierPool::Deallocate(const Id& id)
{
	o_assert_dbg(this->isValid);
	this->freeSlots.Enqueue(id.SlotIndex);
}
