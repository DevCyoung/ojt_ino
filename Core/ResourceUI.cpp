#include "pch.h"
#include "ResourceUI.h"
//itemUI name in resoruce relative file path
void ResourceUI::AddResourceUI(ItemUI* const itemUI)
{
	std::string relativePath = itemUI->GetName();

	const std::wstring& wRelativePath = StringHelper::StrToWStr(relativePath);
	std::wstring wOutRootName;
	std::wstring wOutFilePath;

	StringHelper::GetRootNameAndRelativeFilePath(wRelativePath, &wOutRootName, &wOutFilePath);

	//myRootName
	mRootName = StringHelper::WStrToStr(wOutRootName);
	std::string outFilePath = StringHelper::WStrToStr(wOutFilePath);
}
