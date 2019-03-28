#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"


//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

/**
	Manages the health of a unit. If the health reaches zero, the entity is immediately destroyed.
	Use attack and heal messages to affect the health.

	@author Filip Renman
	@author Philip Stenmark
*/
class HealthComponent : public BaseComponent
{
	ECSComponentType(HealthComponent);
public:
	HealthComponent(int maxHealth);
	void OnLoad();
	void OnReceiveMessage(const Ptr<Message>& message);
	int getHp();

	/// sets a new maximum health limit
	void SetMaxHealth(int maxHealth);
private:
	int maxHealth;
	int currentHealth;
};