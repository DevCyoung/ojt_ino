#include "pch.h"
#include "EngineFile.h"
#include "PathManager.h"

namespace helper
{
	constexpr int MAX_FILE_PATH_SIZE = 256;

	//const wchar_t
	constexpr	LPCWSTR FILTER_TEX = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";

	DialogPath helper::OpenDialog()
	{
		OPENFILENAME ofn = {};
		std::wstring strTileFolderPath = PathManager::GetInstance()->GetResourcePath();

		wchar_t szFilePath[MAX_FILE_PATH_SIZE] = {};

		ZeroMemory(&ofn, sizeof(ofn));														//
		ofn.lStructSize = sizeof(ofn);														//
		ofn.hwndOwner = NULL;																//부모 윈도우핸들
		ofn.lpstrFile = szFilePath;															//
		ofn.lpstrFile[0] = L'\0';															//
		ofn.nMaxFile = MAX_FILE_PATH_SIZE;													//
		ofn.lpstrFilter = FILTER_TEX;														//실습용 
		ofn.nFilterIndex = 1;																//기본필터지정
		ofn.lpstrFileTitle = NULL;															//
		ofn.nMaxFileTitle = 0;																//
		ofn.lpstrInitialDir = strTileFolderPath.c_str();									//매번 필요없는경로를열면 비효율적이기때문에 그곳을보여줌
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;									//

		DialogPath pathDialog = {};

		//다이어로그열기
		if (false == GetOpenFileName(&ofn))
		{
			pathDialog.path = L"NULL";
		}
		else
		{
			pathDialog.path = szFilePath;
		}

		return pathDialog;
	}
	DialogPath helper::SaveDialog()
	{
		OPENFILENAME ofn = {};
		std::wstring strTileFolderPath = PathManager::GetInstance()->GetResourcePath();

		wchar_t szFilePath[MAX_FILE_PATH_SIZE] = {};

		ZeroMemory(&ofn, sizeof(ofn));														//
		ofn.lStructSize = sizeof(ofn);														//
		ofn.hwndOwner = NULL;																//
		ofn.lpstrFile = szFilePath;															//
		ofn.lpstrFile[0] = '\0';															//
		ofn.nMaxFile = MAX_FILE_PATH_SIZE;													//
		ofn.lpstrFilter = FILTER_TEX;														//실습용 
		ofn.nFilterIndex = 1;																//기본필터지정
		ofn.lpstrFileTitle = NULL;															//
		ofn.nMaxFileTitle = 0;																//
		ofn.lpstrInitialDir = strTileFolderPath.c_str();									//매번 필요없는경로를열면 비효율적이기때문에 그곳을보여줌
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;									//

		DialogPath pathDialog = {};

		//다이어로그열기
		if (false == GetSaveFileName(&ofn))
		{
			pathDialog.path = L"NULL";
		}
		else
		{
			pathDialog.path = szFilePath;
		}

		return pathDialog;
	}
}

namespace helper
{
	FileWriter::FileWriter(const DialogPath& path)
		: mFile(nullptr)
	{
		_wfopen_s(&mFile, path.path.c_str(), L"wb");

		Assert(mFile, ASSERT_MSG_INVALID);
	}

	FileWriter::~FileWriter()
	{
		Assert(mFile, ASSERT_MSG_INVALID);

		fclose(mFile);	
	}

	void FileWriter::Write(const void* const buffer, const UINT elementSize)
	{
		fwrite(buffer, elementSize, 1, mFile);
	}

	void FileWriter::WriteLine(const std::wstring& str)
	{
		fwrite(str.c_str(), sizeof(wchar_t), str.length(), mFile);
		fputwc(L'\n', mFile);
	}
}
