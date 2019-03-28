#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Entity class, used to access the entity the component is attached to
#include "ECS/Entity.h"

/**
	This component is used to determine the team of an entity.
	May also be compared directly with other components using overloaded operators.

	@author Philip Stenmark
*/
class TeamTag : public BaseComponent
{
	ECSComponentType(TeamTag);
public:
	enum Code
	{
		Team1 = 1,
		Team2 = 2,
	};

	/// the team
	int Team;

	inline TeamTag(TeamTag::Code id) : Team(static_cast<int>(id)) {};

	inline TeamTag(int team) 
	{
		o_assert_dbg(team == 1 || team == 2);
		this->Team = team;
	};

	inline void OnLoad()
	{
		// allows for easier access
		entity->RegisterAttribute("team", &this->Team, Attribute::Read);
	}

	inline bool operator==(const TeamTag& other)
	{
		return this->Team == other.Team;
	}

	inline bool operator!=(const TeamTag& other)
	{
		return !operator==(other);
	}
};