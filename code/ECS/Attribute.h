#pragma once
#include "glm/glm.hpp"
#include "Core/String/StringAtom.h"

using namespace Oryol;

/*
	A variant class used to contain component attribute pointers.
	This allows for easy variable access across different components.
*/
class Attribute
{
public:
	enum Access
	{
		Read = 0x1,
		Write = 0x2,
		ReadWrite = 0x3,
	};

	enum Type
	{
		// built-in types
		Bool,
		Int,
		Float,
		// specialized types
		String,
		Vector3,
		Vector4,
		Quaternion,
		Matrix4,
	};

	Attribute(bool* ptr);
	Attribute(int* ptr);
	Attribute(float* ptr);
	Attribute(StringAtom* ptr);
	Attribute(glm::vec3* ptr);
	Attribute(glm::vec4* ptr);
	Attribute(glm::quat* ptr);
	Attribute(glm::mat4* ptr);

	/// sets the data access mode
	void SetAccess(const Access& access);

	/// fetch the attribute data type
	Attribute::Type GetType() const;

	/// checks if the attribute is open for reading
	bool IsReadable() const;

	/// checks if the attribute is open for writing
	bool IsWriteable() const;

	/// returns the targeted data type
	template<typename T>
	T& As() const;

private:
	Type type;
	Access access;
	union
	{
		bool* boolPtr;
		int* intPtr;
		float* floatPtr;
		StringAtom* stringPtr;
		glm::vec3* vec3Ptr;
		glm::vec4* vec4Ptr;
		glm::quat* quatPtr;
		glm::mat4* mat4Ptr;
	};
};

inline Attribute::Attribute(bool* ptr) : boolPtr(ptr), type(Bool), access(ReadWrite) {}
inline Attribute::Attribute(int* ptr) : intPtr(ptr), type(Int), access(ReadWrite) {}
inline Attribute::Attribute(float* ptr) : floatPtr(ptr), type(Float), access(ReadWrite) {}
inline Attribute::Attribute(StringAtom* ptr) : stringPtr(ptr), type(String), access(ReadWrite) {}
inline Attribute::Attribute(glm::vec3* ptr) : vec3Ptr(ptr), type(Vector3), access(ReadWrite) {}
inline Attribute::Attribute(glm::vec4* ptr) : vec4Ptr(ptr), type(Vector4), access(ReadWrite) {}
inline Attribute::Attribute(glm::quat* ptr) : quatPtr(ptr), type(Quaternion), access(ReadWrite) {}
inline Attribute::Attribute(glm::mat4* ptr) : mat4Ptr(ptr), type(Matrix4), access(ReadWrite) {}

inline void Attribute::SetAccess(const Access & access)
{
	o_assert_dbg(this->access & access);
	this->access = access;
}

inline Attribute::Type Attribute::GetType() const
{
	return this->type;
}

inline bool Attribute::IsReadable() const
{
	return access & Access::Read;
}

inline bool Attribute::IsWriteable() const
{
	return access & Access::Write;
}

template<typename T>
inline T& Attribute::As() const
{
	o_assert2_dbg(false, "Undefined attribute data type");
	return T();
}

template<>
inline bool& Attribute::As() const
{
	o_assert_dbg(this->type == Type::Bool);
	return *this->boolPtr;
}

template<>
inline int& Attribute::As() const
{
	o_assert_dbg(this->type == Type::Int);
	return *this->intPtr;
}

template<>
inline float& Attribute::As() const
{
	o_assert_dbg(this->type == Type::Float);
	return *this->floatPtr;
}

template<>
inline StringAtom& Attribute::As() const
{
	o_assert_dbg(this->type == Type::String);
	return *this->stringPtr;
}

template<>
inline glm::vec3& Attribute::As() const
{
	o_assert_dbg(this->type == Type::Vector3);
	return *this->vec3Ptr;
}

template<>
inline glm::vec4& Attribute::As() const
{
	o_assert_dbg(this->type == Type::Vector4);
	return *this->vec4Ptr;
}

template<>
inline glm::quat& Attribute::As() const
{
	o_assert_dbg(this->type == Type::Quaternion);
	return *this->quatPtr;
}

template<>
inline glm::mat4& Attribute::As() const
{
	o_assert_dbg(this->type == Type::Matrix4);
	return *this->mat4Ptr;
}