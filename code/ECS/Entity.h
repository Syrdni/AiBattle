#pragma once
#include "Core/Containers/StaticArray.h"
#include "Core/Containers/ArrayMap.h"
#include "Core/String/StringAtom.h"
#include "Core/RefCounted.h"
#include "Resource/Id.h"
#include "ECSUtil.h"
#include "Message.h"
#include "BaseComponent.h"
#include "Attribute.h"

using namespace Oryol;

class Entity : public RefCounted
{
	OryolClassDecl(Entity);
	ECSComponentOwner(Entity);
public:
	/// updates all components of this entity
	void Update();

	/// updates all components of this entity at a fixed time step
	void FixedUpdate();

	/// called when this entity is removed
	void OnDestroy();

	/// called when a message was received by this entity
	void OnReceiveMessage(const Ptr<Message>& message);

	/// adds a component to this entity
	template<typename T, typename... ARGS>
	Ptr<T> AddComponent(ARGS&&... args);

	/// removes a component from this entity
	template<typename T>
	void RemoveComponent();

	/// retrieve a component
	template<typename T>
	Ptr<T> GetComponent() const;

	/// checks if this entity has given component type
	template<typename T>
	bool HasComponent() const;

	/// registers a component attribute to this entity
	template<typename T>
	void RegisterAttribute(const StringAtom& key, T* valuePtr, Attribute::Access mode = Attribute::ReadWrite);

	/// retrieves an attribute type from the system
	template<typename T>
	T GetAttribute(const StringAtom& key) const;

	/// sets an attribute (must be of same type as targeted attribute)
	template<typename T>
	void SetAttribute(const StringAtom& key, T value);

	Id Id;
private:
	StaticArray<Ptr<BaseComponent>, ECSConfig::MaxNumComponents> components;
	ArrayMap<StringAtom, Attribute> attributes;
};

template<typename T, typename... ARGS>
inline Ptr<T> Entity::AddComponent(ARGS&&... args)
{
	o_assert_dbg(!this->HasComponent<T>());
	Ptr<T> component(Memory::New<T>(std::forward<ARGS>(args)...));
	this->components[GetComponentTypeId<T>()] = component;
	component->entity = this;
	component->OnLoad();
	return component;
}

template<typename T>
inline void Entity::RemoveComponent()
{
	o_assert_dbg(this->HasComponent<T>());
	auto type = GetComponentTypeId<T>();
	this->components[type]->OnDestroy();
	this->components[type] = nullptr;
}

template<typename T>
inline Ptr<T> Entity::GetComponent() const
{
	o_assert_dbg(this->HasComponent<T>());
	return this->components[GetComponentTypeId<T>()]->DynamicCast<T>();
}

template<typename T>
inline bool Entity::HasComponent() const
{
	return this->components[GetComponentTypeId<T>()] != nullptr;
}

template<typename T>
inline void Entity::RegisterAttribute(const StringAtom & key, T * valuePtr, Attribute::Access mode)
{
	Attribute attribute = valuePtr;
	attribute.SetAccess(mode);
	this->attributes.Add(key, attribute);
}

template<typename T>
inline T Entity::GetAttribute(const StringAtom & key) const
{
	o_assert2_dbg(this->attributes.Contains(key) && this->attributes[key].IsReadable(), key.AsCStr());
	return this->attributes[key].As<T>();
}

template<typename T>
inline void Entity::SetAttribute(const StringAtom & key, T value)
{
	o_assert2_dbg(this->attributes.Contains(key) && this->attributes[key].IsWriteable(), key.AsCStr());
	auto& attribute = this->attributes[key].As<T>();
	attribute = value;
}
