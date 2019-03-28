#include "Entity.h"

void Entity::Update()
{
	for (auto& c : this->components)
	{
		if (c != nullptr)
			c->Update();
	}
}

void Entity::FixedUpdate()
{
	for (auto& c : this->components)
	{
		if (c != nullptr)
			c->FixedUpdate();
	}
}

void Entity::OnDestroy()
{
	for (auto& c : this->components)
	{
		if (c != nullptr)
			c->OnDestroy();
	}
}

void Entity::OnReceiveMessage(const Ptr<Message>& message)
{
	for (auto& c : this->components)
	{
		if(c != nullptr)
			c->OnReceiveMessage(message);
	}
}