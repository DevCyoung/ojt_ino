#pragma once
#include "ItemUI.h"


//Files

class ResourceUI : public ItemUI
{
public:
	const std::string& GetRootName() { return mRootName; }
	void AddResourceUI(ItemUI* const itemUI);

private:
	std::string mRootName; //������ �ִ� ��Ʈ
};

