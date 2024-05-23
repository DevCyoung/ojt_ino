#include "pch.h"
#include "Entity.h"

Entity::Entity()
	: mName(L"None")
	, mID(sID++)
{
	Assert(mID != UINT_MAX, ASSERT_MSG_INVALID);
}

Entity::~Entity()
{
}

Entity::Entity(const Entity& other)
	: mName(other.mName)
	, mID(sID++)
{
	Assert(mID != UINT_MAX, ASSERT_MSG_INVALID);
}
