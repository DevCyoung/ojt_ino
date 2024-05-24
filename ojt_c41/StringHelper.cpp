#include "pch.h"
#include "StringHelper.h"
#include <filesystem>

namespace fs = std::filesystem;

StringHelper::StringHelper()
{
}

StringHelper::~StringHelper()
{
}

void SaveWString(const std::wstring& str, FILE* file)
{
	UINT len = static_cast<UINT>(str.length());
	fwrite(&len, sizeof(UINT), 1, file);
	fwrite(str.c_str(), sizeof(wchar_t), str.length(), file);
}

void LoadWString(std::wstring* const str, FILE* _File)
{
	wchar_t szBuffer[512] = {};

	UINT iLen = 0;
	fread(&iLen, sizeof(UINT), 1, _File);
	Assert(iLen < 512, ASSERT_MSG_INVALID);
	fread(szBuffer, sizeof(wchar_t), iLen, _File);
	*str = szBuffer;
}



//FIXME ÇÙ
std::wstring StringHelper::StrToWStr(const std::string& str)
{
	const int WSTR_LEN = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0) - 1;
	Assert(0 <= WSTR_LEN, ASSERT_MSG_INVALID);

	std::wstring wstr(WSTR_LEN, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.data(), WSTR_LEN);
	return wstr;
}

std::string StringHelper::WStrToStr(const std::wstring& wstr)
{
	const int STR_LEN = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr) - 1;
	Assert(0 <= STR_LEN, ASSERT_MSG_INVALID);

	std::string str(STR_LEN, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str.data(), STR_LEN, nullptr, nullptr);
	return str;
}

std::vector<std::wstring> StringHelper::VecToWVec(const std::vector<std::string>& vec)
{
	std::vector<std::wstring> result;
	result.reserve(vec.size());

	for (const std::string& str : vec)
	{
		result.push_back(std::wstring(str.begin(), str.end()));
	}
	return result;
}

std::vector<std::string> StringHelper::WVecToVec(const std::vector<std::wstring>& wvec)
{
	std::vector<std::string> result;
	result.reserve(wvec.size());

	for (const std::wstring& wstr : wvec)
	{
		result.push_back(StringHelper::WStrToStr(wstr));
	}
	return result;
}

int StringHelper::GetIndex(const std::vector<std::wstring>& vec, const std::wstring& name)
{
	int idx = -1;
	for (int i = 0; i < vec.size(); ++i)
	{
		if (vec[i] == name)
		{
			idx = i;
			break;
		}
	}
	return idx;
}

std::wstring StringHelper::GetFileExtension(const std::wstring& path)
{	
	constexpr UINT FILE_PATH_MAX_LEN = 512;
	Assert(path.length() < FILE_PATH_MAX_LEN, ASSERT_MSG("error file path"));
	wchar_t szExtension[FILE_PATH_MAX_LEN] = { 0, };

	errno_t err = _wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, FILE_PATH_MAX_LEN);
	(void)(err);
	Assert(!err, ASSERT_MSG("error file path"));

	return szExtension;
}

void StringHelper::GetRootPathAndFileName(const std::wstring& path,
	std::wstring* const outDirectoryPath, 
	std::wstring* const outFileName)
{
	Assert(outDirectoryPath, ASSERT_MSG_NULL);
	Assert(outFileName, ASSERT_MSG_NULL);
	
	std::wstring retDirectoryPath;
	std::wstring retFileName;

	retDirectoryPath.reserve(100);
	retFileName.reserve(100);

	int i = static_cast<int>(path.length()) - 1;

	if (L'\\' == path[i])
	{
		--i;
	}

	for (; i >= 0; --i)
	{
		if (L'\\' == path[i])
		{
			break;
		}

		retFileName += path[i];
	}

	for (; i >= 0; --i)
	{
		retDirectoryPath += path[i];
	}

	std::reverse(retFileName.begin(), retFileName.end());
	std::reverse(retDirectoryPath.begin(), retDirectoryPath.end());

	*outDirectoryPath = retDirectoryPath;
	*outFileName = retFileName;
}

void StringHelper::GetRootNameAndRelativeFilePath(const std::wstring& path,
	std::wstring* const outRootName, 
	std::wstring* const outFilePath)
{
	Assert(outRootName, ASSERT_MSG_NULL);
	Assert(outFilePath, ASSERT_MSG_NULL);

	constexpr UINT FILE_PATH_MAX_LEN = 512;
	int check = 0;

	std::wstring retRootName;
	std::wstring retFilePath;

	retRootName.reserve(FILE_PATH_MAX_LEN);
	retFilePath.reserve(FILE_PATH_MAX_LEN);

	int i = 0;

	for (; i < path.length(); ++i)
	{
		if (path[i] == L'\\')
		{
			++check;
		}
		if (2 == check)
		{
			break;
		}

		retRootName += path[i];
	}

	for (; i < path.length(); ++i)
	{
		retFilePath += path[i];
	}

	//ÆÄÀÏÈ¥ÀÚ³²¾Ò´Ù!
	if (retFilePath.empty())
	{
		retFilePath = retRootName;
		retRootName.clear();
	}

	*outRootName = retRootName;
	*outFilePath = retFilePath;
}

std::wstring StringHelper::SWPrintf(const wchar_t* const format, ...)
{
	constexpr UINT STRING_MAX_LEN = 512;

	wchar_t szBuffer[STRING_MAX_LEN] = { 0, };

	va_list args;
	va_start(args, format);
	swprintf_s(szBuffer, STRING_MAX_LEN, format, args);
	va_end(args);

	return szBuffer;
}

std::wstring StringHelper::GetDirectoryNameFromPath(const std::wstring& filePath)
{
	fs::path pathObj(filePath);

	if (fs::is_directory(pathObj)) 
	{
		return pathObj.filename().wstring();
	}

	fs::path dirPath = pathObj.parent_path();
	return dirPath.filename().wstring();
}

std::wstring StringHelper::GetFileNameFromPath(const std::wstring& filePath)
{
	fs::path pathObj(filePath);	
	return pathObj.filename().wstring();
}
