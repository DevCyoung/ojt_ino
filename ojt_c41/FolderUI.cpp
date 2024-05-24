#include "pch.h"
#include "FolderUI.h"

FolderUI::FolderUI()
{
}

FolderUI::~FolderUI()
{
	mem::del::DeleteVectorElements(&mFiles);
	mem::del::DeleteVectorElements(&mForderUIs);
}

void FolderUI::AddResourceUI(ResourceUI* const resourceUI)
{
	std::string relativePath = resourceUI->GetName();

	const std::wstring& wRelativePath = StringHelper::StrToWStr(relativePath);
	std::wstring wOutRootName;
	std::wstring wOutFilePath;

	StringHelper::GetRootNameAndRelativeFilePath(wRelativePath, &wOutRootName, &wOutFilePath);

	std::string outRootName = StringHelper::WStrToStr(wOutRootName);
	std::string outFilePath = StringHelper::WStrToStr(wOutFilePath);

	resourceUI->SetName(outFilePath);

	if (1 < outRootName.length())
	{
		FolderUI* folderUI = FindFolderUIOrNull(outRootName);
		if (nullptr == folderUI)
		{
			//Textuer¸¦ Ã³À½º½
			folderUI = new FolderUI();
			folderUI->mForderName = outRootName;
			mForderUIs.push_back(folderUI);
			folderUI->AddResourceUI(resourceUI);
		}
		else
		{
			folderUI->AddResourceUI(resourceUI);
		}
	}
	else
	{
		mFiles.push_back(resourceUI);
	}
}

//one slash
bool FolderUI::IsFileName(std::string fileName)
{
	//slash count
	int count = 0;

	for (int i = 0; i < fileName.length(); i++)
	{
		if (fileName[i] == '\\')
		{
			++count;
		}

		if (count > 1)
		{
			return false;
		}

	}

	return true;
}

FolderUI* FolderUI::FindFolderUIOrNull(std::string forderName)
{
	for (FolderUI* forderUI : mForderUIs)
	{
		if (forderUI->mForderName == forderName)
		{
			return forderUI;
		}
	}
	return nullptr;
}

void FolderUI::render()
{
	for (ResourceUI* resourceUI : mFiles)
	{		
		resourceUI->render();
	}
	
	for (FolderUI* folder : mForderUIs)
	{						
		if (ImGui::TreeNode(folder->mForderName.c_str()))
		{			
			folder->render();
			ImGui::TreePop();
		}		
	}
}

