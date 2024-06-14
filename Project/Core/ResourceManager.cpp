#include "pch.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	mem::del::DeleteUnorderedMapArrayElements(&mResourceMapArray);	
}

const ResourceManager::Dictionary& ResourceManager::GetDictionary(const eResourceType type)
{
	return mResourceMapArray[static_cast<UINT>(type)];
}
