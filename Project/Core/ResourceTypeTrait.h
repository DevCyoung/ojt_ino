#pragma once
#include "EnumResource.h"

template <typename T>
struct engine_resource_type
{
	static constexpr bool value = false;
};

#define ENGINE_RESOURCE_TYPE(RES_TYPE)									\
class RES_TYPE;															\
enum class eRes##RES_TYPE;												\
template<>																\
struct engine_resource_type<RES_TYPE>									\
{																		\
	static constexpr bool value = true;									\
	using eResEnumType = eRes##RES_TYPE;								\
	static constexpr eResourceType type = eResourceType::##RES_TYPE;	\
}																		\

ENGINE_RESOURCE_TYPE(Texture);
ENGINE_RESOURCE_TYPE(Mesh);
ENGINE_RESOURCE_TYPE(Material);
ENGINE_RESOURCE_TYPE(Shader);
ENGINE_RESOURCE_TYPE(Animation);
ENGINE_RESOURCE_TYPE(Font);
ENGINE_RESOURCE_TYPE(Sound);
ENGINE_RESOURCE_TYPE(Scene);
ENGINE_RESOURCE_TYPE(Prefab);
ENGINE_RESOURCE_TYPE(ComputeShader);
ENGINE_RESOURCE_TYPE(AudioClip);
ENGINE_RESOURCE_TYPE(MeshData);
//ENGINE_RESOURCE_TYPE(FBX);

template <typename T>
struct is_engine_resource
{
	static constexpr bool value = engine_resource_type<T>::value;	
};
