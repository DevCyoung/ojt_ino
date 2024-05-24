#pragma once
#include "Resource.h"
#include "PathManager.h"
#include "ResourceTypeTrait.h"

class ResourceManager
{
	friend class Engine;
	SINGLETON_DECLARE(ResourceManager);

public:
	using Key = std::wstring;
	using Value = Resource;
	using Dictionary = std::unordered_map<Key, Value*>;
	using Iterator = Dictionary::iterator;
	using ConstIterator = Dictionary::const_iterator;

public:
	template<typename T>
		requires (is_engine_resource<T>::value)
	T* FindOrNull(const Key& relativePathOrName) const ;

	template<typename T>
		requires (is_engine_resource<T>::value)
	T* Find(const Key& relativePathOrName);

	template <typename T>
		requires (is_engine_resource<T>::value)
	void Load(const Key& relativePathOrName);

	template<typename T>
		requires (is_engine_resource<T>::value)
	T* FindAndLoad(const Key& relativePathOrName);

	template<typename T>
		requires (is_engine_resource<T>::value)
	T* FindAndLoadOrNull(const Key& relativePathOrName);

	template<typename T>
		requires (is_engine_resource<T>::value)
	void Insert(const Key& relativePathOrName, T* const value);

	const Dictionary&  GetDictionary(const eResourceType type);

	template<typename T>
		requires (is_engine_resource<T>::value)
	ConstIterator  CBegine();

	template<typename T>
		requires (is_engine_resource<T>::value)
	ConstIterator  CEnd();

	template<typename T>
		requires (is_engine_resource<T>::value)
	Iterator  Begine();

	template<typename T>
		requires (is_engine_resource<T>::value)
	Iterator  End();

private:
	Dictionary mResourceMapArray[static_cast<UINT>(eResourceType::End)];
};
#define gResourceManager ResourceManager::GetInstance()

template<typename T>
	requires (is_engine_resource<T>::value)
inline T* ResourceManager::FindOrNull(const Key& relativePathOrName) const
{
	T* res = nullptr;

	constexpr eResourceType RES_TYPE = engine_resource_type<T>::type;
	const Dictionary& RESOURCES = mResourceMapArray[static_cast<UINT>(RES_TYPE)];
	ConstIterator iter = RESOURCES.find(relativePathOrName);

	if (iter != RESOURCES.end())
	{
		res = dynamic_cast<T*>(iter->second);
		Assert(res, ASSERT_MSG_NULL);
	}

	return res;
}


template<typename T>
	requires (is_engine_resource<T>::value)
inline T* ResourceManager::Find(const Key& relativePathOrName)
{
	T* resource = FindOrNull<T>(relativePathOrName);

	if (nullptr == resource)
	{
		Load<T>(relativePathOrName);
		resource = FindOrNull<T>(relativePathOrName);
	}

	Assert(resource, ASSERT_MSG_NULL);
	return resource;
}


template<typename T>
	requires (is_engine_resource<T>::value)
inline void ResourceManager::Load(const Key& relativePathOrName)
{	
	//Assert(!FindOrNull<T>(relativePathOrName), ASSERT_MSG_NOT_NULL);
	T* resource = FindOrNull<T>(relativePathOrName);
	if (resource)
	{
		return;
	}

	resource = new T();
	constexpr eResourceType RES_TYPE = engine_resource_type<T>::type;
	Dictionary& resources = mResourceMapArray[static_cast<UINT>(RES_TYPE)];
	const std::wstring FILE_PATH = PathManager::GetInstance()->GetResourcePath() + relativePathOrName;

	resource->Load(FILE_PATH);
	resource->mRelativePathOrName = relativePathOrName;
	resources.insert(std::make_pair(relativePathOrName, resource)); //key : relative Path
}

template<typename T>
	requires (is_engine_resource<T>::value)
T* ResourceManager::FindAndLoad(const Key& relativePathOrName)
{
	T* resource = FindAndLoadOrNull<T>(relativePathOrName);

	Assert(resource, ASSERT_MSG_NULL);

	return resource;
}

template<typename T>
	requires (is_engine_resource<T>::value)
inline T* ResourceManager::FindAndLoadOrNull(const Key& relativePathOrName)
{
	T* resource = FindOrNull<T>(relativePathOrName);
	if (nullptr == resource)
	{
		Load<T>(relativePathOrName);
		resource = FindOrNull<T>(relativePathOrName);
	}

	return resource;
}

template<typename T>
	requires (is_engine_resource<T>::value)
inline void ResourceManager::Insert(const Key& relativePathOrName, T* const value)
{
	Assert(value, ASSERT_MSG_NULL);
	Assert(!(FindOrNull<T>(relativePathOrName)), ASSERT_MSG_NOT_NULL);

	value->mRelativePathOrName = relativePathOrName;

	constexpr eResourceType RES_TYPE = engine_resource_type<T>::type;
	Dictionary& resources = mResourceMapArray[static_cast<UINT>(RES_TYPE)];

	resources.insert(std::make_pair(relativePathOrName, value));
}

template<typename T>
	requires (is_engine_resource<T>::value)
inline ResourceManager::ConstIterator  ResourceManager::CBegine()
{
	constexpr eResourceType RES_TYPE = engine_resource_type<T>::type;
	return  mResourceMapArray[static_cast<UINT>(RES_TYPE)].cbegin();
}

template<typename T>
	requires (is_engine_resource<T>::value)
inline ResourceManager::ConstIterator  ResourceManager::CEnd()
{
	constexpr eResourceType RES_TYPE = engine_resource_type<T>::type;
	return  mResourceMapArray[static_cast<UINT>(RES_TYPE)].cend();
}

template<typename T>
	requires (is_engine_resource<T>::value)
inline ResourceManager::Iterator  ResourceManager::Begine()
{
	constexpr eResourceType RES_TYPE = engine_resource_type<T>::type;
	return  mResourceMapArray[static_cast<UINT>(RES_TYPE)].begin();
}

template<typename T>
	requires (is_engine_resource<T>::value)
inline ResourceManager::Iterator  ResourceManager::End()
{
	constexpr eResourceType RES_TYPE = engine_resource_type<T>::type;
	return  mResourceMapArray[static_cast<UINT>(RES_TYPE)].end();
}
