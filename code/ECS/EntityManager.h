#pragma once
#include "Core/Containers/Map.h"
#include "IdentifierPool.h"
#include "Entity.h"
#include "Core/Time/Clock.h"

class EntityManager
{
public:
	/// sets up containers
	EntityManager();

	/// destroys all entities
	void Discard();

	/// updates all entities and their components
	void Update();

	/// creates a new entity object
	Ptr<Entity> CreateEntity();

	/// removes an entity
	void RemoveEntity(const Id& id);

	/// retrieves an entity by idenfitier
	Ptr<Entity> GetEntity(const Id& id) const;

	bool IsValid(const Id& id) const;

	/// get the number of active entities
	int GetNumEntities() const;

	/// number of ticks per second
	static const int TickRate = 60;
private:
	Map<Id, Ptr<Entity>> entities;
	IdentifierPool idPool;

	TimePoint lastTime;
	Duration TimeSinceLastUpdate();
	double msPerTick;
};