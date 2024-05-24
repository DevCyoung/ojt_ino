#pragma once

enum class eResourceType;

class Resource
{
	friend class ResourceManager;
public:
	Resource();
	Resource(const Resource& other) = default;
	Resource(eResourceType mResourceType);
	virtual ~Resource();

	virtual HRESULT Load(const std::wstring& filePath) = 0;
	virtual HRESULT Save(const std::wstring& filePath) { (void)filePath; return S_OK; }

	const std::wstring& GetRelativePathorName() const { return mRelativePathOrName; }

	eResourceType GetType() { return mResourceType; }

protected:
	void SetRelativePath(const std::wstring& relativePath) { mRelativePathOrName = relativePath; }

private:
	eResourceType mResourceType;
	std::wstring mRelativePathOrName;
};