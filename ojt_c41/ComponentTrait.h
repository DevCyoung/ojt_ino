#pragma once
#include "EngineComponentTrait.h"
#include "ScriptComponentTrait.h"

template <typename T>
struct is_engine_component
{
	static constexpr bool value = engine_component_trait<T>::value;
};

template <typename T>
struct is_script_component
{
	static constexpr bool value = script_component_trait<T>::value;
};

template <typename T>
struct is_component
{
	static constexpr bool value = (is_engine_component<T>::value ||
		is_script_component<T>::value);
};
