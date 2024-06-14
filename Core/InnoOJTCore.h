#pragma once

#include <assert.h>
#include <Windows.h>
#include <string>
#include <vector> 
#include <stack>
#include <list> 
#include <map> 
#include <queue>
#include <unordered_map> 
#include <bitset> 
#include <set> 
#include <functional> 
#include <cmath> 
#include <algorithm> 
#include <limits>
#include <memory> 
#include <filesystem> 
#include <wrl.h>
#include <commdlg.h>
#include <d3d11.h>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_helper.h"
#include "StringHelper.h"
#include "DXMath.h"
#include "DirectXTex.h"
#include "Protocal.h"

using namespace engine::math;

#define DELETE_POINTER(p)		{ delete   p; p = nullptr; }
#define DELETE_ARRAY(p)			{ delete[] p; p = nullptr; }

#define DELETE_POINTER_NOT_NULL(p)  { Assert(p, ASSERT_MSG_NULL); DELETE_POINTER(p) }
#define DELETE_ARRAY_NOT_NULL(p)	{ Assert(p, ASSERT_MSG_NULL); DELETE_ARRAY(p) }

namespace mem
{
	template<typename T, UINT Size>
	void DeletePointerArrayElements(T* (* const pPointerArray)[Size])
	{
		for (auto& value : (*pPointerArray))
		{
			DELETE_POINTER(value);
		}
	}

	template<typename T>
	void DeleteMapElements(T* const pMap)
	{
		for (auto& value : (*pMap))
		{
			DELETE_POINTER(value.second);
		}

		pMap->clear();
	}

	template<typename T>
	void DeleteContainerElements(T* const pContainer)
	{
		for (auto& value : (*pContainer))
		{
			DELETE_POINTER(value);
		}

		pContainer->clear();
	}
}

namespace mem::del
{
	template<typename T, UINT Size>
	void DeletePointerArrayElements(T* (* const pPointerArray)[Size])
	{
		mem::DeletePointerArrayElements(pPointerArray);
	}

	template<typename Pointer>
	void DeleteVectorElements(std::vector<Pointer>* const pVector)
	{
		mem::DeleteContainerElements(pVector);
	}

	template<typename Key, typename Pointer>
	void DeleteMapElements(std::map<Key, Pointer>* const map)
	{
		mem::DeleteMapElements(map);
	}

	template<typename Key, typename Pointer>
	void DeleteUnorderedMapElements(std::unordered_map<Key, Pointer>* const pUnorderedMap)
	{
		mem::DeleteMapElements(pUnorderedMap);
	}

	template<typename Pointer, UINT Size>
	void DeleteVectorArrayElements(std::vector<Pointer>(* const pVectorArray)[Size])
	{
		for (auto& value : (*pVectorArray))
		{
			DeleteVectorElements(&value);
		}
	}

	template<typename Key, typename Pointer, UINT Size>
	void DeleteMapArrayElements(std::map<Key, Pointer>(* const pMapArray)[Size])
	{
		for (auto& value : (*pMapArray))
		{
			DeleteMapElements(&value);
		}
	}

	template<typename Key, typename Pointer, UINT Size>
	void DeleteUnorderedMapArrayElements(std::unordered_map<Key, Pointer>(* const pUnorderedMapArray)[Size])
	{
		for (auto& value : (*pUnorderedMapArray))
		{
			DeleteUnorderedMapElements(&value);
		}
	}
}

#include "EnumFlag.h"

#define ASSERT_MSG_SWITCH_DEFAULT "switch default"
#define ASSERT_MSG_NULL "value is nullptr"
#define ASSERT_MSG_NOT_NULL "value is not nullptr"
#define ASSERT_MSG_INVALID "value is invalid"

#define L_PREFIX(quote) L##quote
#define ASSERT_MSG(message) message

#define Assert(expression, message) (void)(expression); assert(expression && L_PREFIX(message))
#define Static_Assert(expression, message) static_assert(expression, message)

#define FIXME(message) MessageBox(nullptr, L_PREFIX(message), L_PREFIX(message), MB_OK | MB_ICONWARNING)

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }



#define SINGLETON_DECLARE(CLASSNAME)						\
private:													\
	static void initialize()								\
	{														\
		Assert(!sInstance, ASSERT_MSG_NOT_NULL);			\
		sInstance = new CLASSNAME();						\
	}														\
static void deleteInstance()								\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		delete sInstance;									\
		sInstance = nullptr;								\
	}														\
public:														\
	static CLASSNAME* GetInstance()							\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		return sInstance;									\
	}														\
private:													\
	CLASSNAME();											\
	virtual ~CLASSNAME();									\
	CLASSNAME(const CLASSNAME&) = delete;					\
	CLASSNAME& operator=(const CLASSNAME&) = delete;		\
private:													\
	inline static CLASSNAME* sInstance = nullptr;			\


#define PUBLIC_SINGLETON_DECLARE(CLASSNAME)					\
public:														\
	static void initialize()								\
	{														\
		Assert(!sInstance, ASSERT_MSG_NOT_NULL);			\
		sInstance = new CLASSNAME();						\
	}														\
static void deleteInstance()								\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		delete sInstance;									\
		sInstance = nullptr;								\
	}														\
public:														\
	static CLASSNAME* GetInstance()							\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		return sInstance;									\
	}														\
private:													\
	CLASSNAME();											\
	virtual ~CLASSNAME();									\
	CLASSNAME(const CLASSNAME&) = delete;					\
	CLASSNAME& operator=(const CLASSNAME&) = delete;		\
private:													\
	inline static CLASSNAME* sInstance = nullptr;			\
