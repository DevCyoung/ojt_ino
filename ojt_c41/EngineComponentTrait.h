#pragma once
enum class eComponentType;

template <typename T>
struct engine_component_trait
{
	static constexpr bool value = false;
};

#define REGISTER_COMPONENT_TYPE(COMPONENT_TYPE)	class COMPONENT_TYPE;							\
template <>																						\
struct engine_component_trait<COMPONENT_TYPE>													\
{																								\
	static constexpr bool value = true;															\
	static constexpr eComponentType type = eComponentType::##COMPONENT_TYPE;					\
	static constexpr const wchar_t* const name = L#COMPONENT_TYPE;								\
}																								\
	