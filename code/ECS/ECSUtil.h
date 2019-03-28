#pragma once
#include "Core/RefCounted.h"

class ECSConfig
{
public:
	static const int MaxNumEntities = 5000;
	static const int MaxNumComponents = 30;
};

constexpr auto EntityResourceType = 0;

#define ECSComponentOwner(CLASS)\
protected:\
	static size_t Next() { static size_t last = 0; return last++; };\
	template<class T> static size_t GetComponentTypeId() { static size_t id = Next(); return id; };

#define ECSComponentType(CLASS)\
	OryolTypeDecl(CLASS, BaseComponent);

#define ECSMessageType(CLASS)\
	OryolClassDecl(CLASS);\
	OryolTypeDecl(CLASS, Message);
