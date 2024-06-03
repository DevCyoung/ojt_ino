#pragma once
#include <string.h>

void SaveWString(const std::wstring& _str, FILE* _File);
void LoadWString(std::wstring* const str, FILE* _File);

class StringHelper
{
private:
	StringHelper();
	virtual ~StringHelper();
	StringHelper(const StringHelper&) = delete;
	StringHelper& operator=(const StringHelper&) = delete;

public:
	static std::vector<std::wstring> VecToWVec(const std::vector<std::string>& vec);
	static std::vector<std::string>  WVecToVec(const std::vector<std::wstring>& wvec);
	static int GetIndex(const std::vector<std::wstring>& vec, const std::wstring& name);

	static std::wstring StrToWStr(const std::string& str);
	static std::string WStrToStr(const std::wstring& wstr);
	static std::wstring GetFileExtension(const std::wstring& filePath);

	static void GetRootPathAndFileName(const std::wstring& filePath,
		std::wstring* const outDirectoryPath,
		std::wstring* const outFileName);

	static void GetRootNameAndRelativeFilePath(const std::wstring& filePath,
		std::wstring* const outRootName,
		std::wstring* const outFilePath);

	static std::wstring SWPrintf(const wchar_t* const format, ...);

	static std::wstring GetDirectoryNameFromPath(const std::wstring& filePath);
	static std::wstring GetFileNameFromPath(const std::wstring& filePath);
};
