#pragma once
#include "ItemUI.h"
#include "ResourceUI.h"

class FolderUI : public ItemUI
{
	std::string mForderName; //ex) \Atlas

	//파일들
	std::vector<ResourceUI*> mFiles;

	//폴더들
	std::vector<FolderUI*> mForderUIs;

	//폴더는 내가만드는게 아니라 자동으로 만들어져야함
public :
	void AddResourceUI(ResourceUI* const resourceUI);
	bool IsFileName(std::string fileName);

	FolderUI* FindFolderUIOrNull(std::string forderName); // \Atla;


	virtual void render() override;

public:
	FolderUI();
	virtual ~FolderUI();
};

