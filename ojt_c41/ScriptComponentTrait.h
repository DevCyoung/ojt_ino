#pragma once
enum class eScriptComponentType;

template <typename T>
struct script_component_trait
{
	static constexpr bool value = false;
};

#define REGISTER_SCRIPTCOMPONENT_TYPE(SCRIPT_TYPE)	class SCRIPT_TYPE;							\
template <>																						\
struct script_component_trait<SCRIPT_TYPE>														\
{																								\
	static constexpr bool value = true;															\
	static constexpr eScriptComponentType type = eScriptComponentType::##SCRIPT_TYPE;			\
	static constexpr const wchar_t* const name = L#SCRIPT_TYPE;									\
}																								\

