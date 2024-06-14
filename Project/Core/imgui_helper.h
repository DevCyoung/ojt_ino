#pragma once
#include <vector>
#include <string>
#include <array>
#include <span>

namespace ImGui
{
	bool Combo(const char* label, int* current_item, const std::vector<std::string>& vector);
	bool Combo(const char* label, int* current_item, const std::vector<std::wstring>& vector);
}