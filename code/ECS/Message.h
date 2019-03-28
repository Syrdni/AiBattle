#pragma once
#include "Core/RefCounted.h"
#include "ECS/ECSUtil.h"

using namespace Oryol;

class Message : public RefCounted
{
	OryolBaseClassDecl(Message);
	OryolBaseTypeDecl(Message);
};