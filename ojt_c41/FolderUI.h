#pragma once
#include "ItemUI.h"
#include "ResourceUI.h"

class FolderUI : public ItemUI
{
	std::string mForderName; //ex) \Atlas

	//���ϵ�
	std::vector<ResourceUI*> mFiles;

	//������
	std::vector<FolderUI*> mForderUIs;

	//������ ��������°� �ƴ϶� �ڵ����� �����������
public :
	void AddResourceUI(ResourceUI* const resourceUI);
	bool IsFileName(std::string fileName);

	FolderUI* FindFolderUIOrNull(std::string forderName); // \Atla;


	virtual void render() override;

public:
	FolderUI();
	virtual ~FolderUI();
};

