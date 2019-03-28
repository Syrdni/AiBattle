#include "EntityManager.h"

EntityManager::EntityManager()
{
	// adjust map capacity
	this->entities.Reserve(ECSConfig::MaxNumEntities);

	// setup identifier pool
	this->idPool.Setup(EntityResourceType, ECSConfig::MaxNumEntities);

	this->msPerTick = 1000.0 / TickRate;
}

void EntityManager::Discard()
{
	for (auto& e : this->entities)
	{
		this->RemoveEntity(e.Key());
	}
}

void EntityManager::Update()
{
	if (this->TimeSinceLastUpdate().AsMilliSeconds() >= msPerTick)
	{
		lastTime = Clock::Now();
		for (auto& e : this->entities)
		{
			if (e.Value() != nullptr)
			{
				e.Value()->FixedUpdate();
			}
		}
	}

	for (auto& e : this->entities)
	{
		if (e.Value() != nullptr)
		{
			e.Value()->Update();
		}
	}
}

Ptr<Entity> EntityManager::CreateEntity()
{
	auto entity = Entity::Create();
	entity->Id = this->idPool.Allocate();
	this->entities.Add(entity->Id, entity);
	return entity;
}

void EntityManager::RemoveEntity(const Id& id)
{
	o_assert_dbg(this->entities.Contains(id) && id.IsValid());
	this->idPool.Deallocate(id);
	this->entities[id]->OnDestroy();
	this->entities.Erase(id);
}

Ptr<Entity> EntityManager::GetEntity(const Id& id) const
{
	o_assert_dbg(this->entities.Contains(id) && id.IsValid());
	return this->entities[id];
}

bool EntityManager::IsValid(const Id& id) const {
	return this->entities.Contains(id) && id.IsValid();
}

int EntityManager::GetNumEntities() const
{
	return this->entities.Size();
}

Duration EntityManager::TimeSinceLastUpdate()
{
	return Clock::Now() - lastTime;
}
