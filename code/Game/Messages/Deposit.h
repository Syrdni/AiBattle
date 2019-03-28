#pragma once
//Message base class
#include "ECS/Message.h"

//Glm
#include "glm/glm.hpp"

struct MaterialsToDeposit {
	bool wood = false;
	bool coal = false;
	bool ironOre = false;
	bool ironIngot = false;
	bool sword = false;
};

struct MaterialsToSend {
	int wood = 0;
	int coal = 0;
	int ironOre = 0;
	int ironIngot = 0;
	int sword = 0;
};

class Deposit : public Message
{
	ECSMessageType(Deposit);
public:
	Deposit(MaterialsToDeposit matsToBeDeposited) {
		materialsToDeposit = matsToBeDeposited;
	};

	MaterialsToDeposit materialsToDeposit;
};


class RequestMaterials : public Message
{
	ECSMessageType(RequestMaterials);

public:
	RequestMaterials(MaterialsToSend mats, const Ptr<Entity>& sender) {
		this->materialsToSend = mats;
		this->sender = sender;
	}
	MaterialsToSend materialsToSend;

	Ptr<Entity> sender;
};

class RequestMaterialsAnswer : public Message{
	ECSMessageType(RequestMaterialsAnswer);

public:
	RequestMaterialsAnswer(bool haveEnoughMaterials) {
		this->answer = haveEnoughMaterials;
	}

	bool answer;
};