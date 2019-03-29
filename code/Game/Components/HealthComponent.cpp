#pragma once
#include "HealthComponent.h"

//Components
#include "LevelVisitor.h"

//Messages used by the component
#include "../Messages/Attack.h"
#include "../Messages/Heal.h"
#include "../Messages/EntityRemove.h"
#include "Soldier.h"
#include "Explorer.h"



//Used to remove entities if the die, also to send log messages to the console
#include "../Game.h"
//class Craftsman;
class Craftsman;
class Worker;
class KilnComponent;
class SmelterComponent;
class SmithyComponent;
class TrainingCampComponent;

HealthComponent::HealthComponent(int maxHealth)
{
	this->maxHealth = maxHealth;
	this->currentHealth = maxHealth;
}

int HealthComponent::getHp()
{
	return currentHealth;
}

void HealthComponent::OnLoad()
{
	entity->RegisterAttribute("health", &this->currentHealth, Attribute::ReadWrite);
	entity->RegisterAttribute("maxHealth", &this->maxHealth, Attribute::ReadWrite);
}

void HealthComponent::OnReceiveMessage(const Ptr<Message>& message)
{
	// take damage by attack
	if (message->IsA<Attack>())
	{
		auto msg = message->DynamicCast<Attack>();
		Game::Console.Log("Entity %i was attacked", entity->Id.SlotIndex);

		this->currentHealth -= msg->DamageAmount;
		if (currentHealth <= 0)
		{
			if (entity->HasComponent<Explorer>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{

					Game::units[1]--;
				}
				else
				{

					Game::units[5]--;
				}
			}
			else if (entity->HasComponent<Soldier>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[3]--;
				}
				else
				{
					Game::units[7]--;
				}
			}
			else if (entity->HasComponent<Worker>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{

					Game::units[0]--;
				}
				else
				{
					Game::units[4]--;

				}
			}
			else if (entity->HasComponent<Craftsman>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[2]--;
				}
				else
				{
					Game::units[6]--;
				}
			}

			else if (entity->HasComponent<KilnComponent>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[8]--;
				}
				else
				{
					Game::units[12]--;
				}
			}
			else if (entity->HasComponent<SmelterComponent>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[9]--;
				}
				else
				{
					Game::units[13]--;
				}
			}
			else if (entity->HasComponent<SmithyComponent>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[10]--;
				}
				else
				{
					Game::units[14]--;
				}
			}
			else if (entity->HasComponent<TrainingCampComponent>())
			{
				if (entity->GetComponent<TeamTag>()->Team == 1)
				{
					Game::units[11]--;
				}
				else
				{
					Game::units[15]--;
				}
			}


			Game::Console.Log("Entity %i was removed", entity->Id.SlotIndex);
			Game::EntityManager.RemoveEntity(entity->Id);
		}
	}

	// refill health by remote sender
	if (message->IsA<Heal>())
	{
		auto msg = message->DynamicCast<Heal>();

		currentHealth += msg->HealAmount;
		if (currentHealth > maxHealth)
		{
			currentHealth = maxHealth;
		}
	}
}

void HealthComponent::SetMaxHealth(int maxHealth)
{
	this->maxHealth = maxHealth;
	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}
}
