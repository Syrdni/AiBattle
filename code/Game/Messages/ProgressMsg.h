#pragma once
//Message base class
#include "ECS/Message.h"

//Component checks
#include "ECS/BaseComponent.h"
#include "ECS/Entity.h"
/*
	Enum for telling an entity what building to upgrade to
	@author Kristoffer Lundgren
*/
enum BuildingTypes {
	noneBuilding,
	kiln,
	smithy,
	trainingCamp,
	smelter
};
/*
	Enum for telling an enityty what type it should upgrade to
	@author Kristoffer Lundgren
*/
enum WorkerTypes {
	noneWorker,
	explorer,
	craftsman,
	solider
};
/*
	Enum for telling the craftsman what resource was just made
	@author Kristoffer Lundgren
*/
enum ResourceTypes {
	sword, 
	charcoal, 
	ironIngot

};

/*
	Message for when the progress timer is complete
	@author Kristoffer Lundgren, Benjamin Vesterlund
*/
class ProgressDone : public Message
{
	ECSMessageType(ProgressDone);
public:
	ProgressDone(Ptr<Entity> entity, int component) { this->entity = entity; this->component = component; };
	Ptr<Entity> entity;
	int component;
};
/*
	Message for starting the progress timer
	@author Kristoffer Lundgren, Benjamin Vesterlund
*/
class StartProgress : public Message
{
	ECSMessageType(StartProgress);
public:
	StartProgress(Ptr<Entity> returnEntity, int component) { this->returnEntity = returnEntity; this->component = component; };


	Ptr<Entity> returnEntity;
	int component;
};

class StartProgressTime : public Message
{
	ECSMessageType(StartProgressTime);
public:
	StartProgressTime(int TimeRequired, Ptr<Entity> returnEntity, int component) { this->TimeRequired = TimeRequired; this->returnEntity = returnEntity; this->component = component; };

	int TimeRequired;
	Ptr<Entity> returnEntity;
	int component;
};
