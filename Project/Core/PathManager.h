#pragma once

class PathManager
{
	friend class Engine;
	SINGLETON_DECLARE(PathManager);

public:
	LPCWSTR	GetResourcePath() const { return mResourcePath; }

private: 
	WCHAR	mResourcePath[MAX_PATH];
};

#define gPathManager PathManager::GetInstance()
