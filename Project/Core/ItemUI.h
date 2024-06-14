
#pragma once

enum class eResourceType;
class Resource;

class ItemUI
{
public:
	ItemUI();
	virtual ~ItemUI();

	void AddChildItemUI(ItemUI* const itemUI)
	{
		Assert(itemUI, ASSERT_MSG_NOT_NULL);
		mChildItemUIs.push_back(itemUI);
	}
	void SetResource(Resource* resource) { mResource = resource; }
	void SetName(const std::string& itemName) { mItemName = itemName; }
	void SetFullName(const std::string& itemName) { mFullName = itemName; }
	const std::string& GetName() { return mItemName; };
	ItemUI* FindItemUIOrNull(const std::string& itemUIName);

public:
	virtual void render();

private:
	//eResourceType	mItemType;
	Resource* mResource;
	std::string mFullName;
	std::string mItemName;
	std::vector<ItemUI*> mChildItemUIs;
};

