#include "pch.h"
#include "EngineFile.h"
#include "PathManager.h"

namespace helper
{
	DialogPath helper::OpenDialog()
	{
		OPENFILENAME ofn = {};
		std::wstring strTileFolderPath = PathManager::GetInstance()->GetResourcePath();

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));														//
		ofn.lStructSize = sizeof(ofn);														//
		ofn.hwndOwner = NULL;																//
		ofn.lpstrFile = szFilePath;															//
		ofn.lpstrFile[0] = '\0';															//
		ofn.nMaxFile = 256;																	//
		ofn.lpstrFilter = nullptr;																//실습용 
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

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));														//
		ofn.lStructSize = sizeof(ofn);														//
		ofn.hwndOwner = NULL;																//
		ofn.lpstrFile = szFilePath;															//
		ofn.lpstrFile[0] = '\0';															//
		ofn.nMaxFile = 256;																	//
		ofn.lpstrFilter = nullptr;																//실습용 
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


//
//
//
//void helper::GetFileForRead(FILE** const file, const DialogPath& path)
//{
//	_wfopen_s(file, path.path.c_str(), L"rb");
//
//	Assert(*file, WCHAR_IS_INVALID_TYPE);
//}
//
//void helper::GetFileForWrite(FILE** const file, const DialogPath& path)
//{
//	_wfopen_s(file, path.path.c_str(), L"wb");
//
//	Assert(*file, WCHAR_IS_INVALID_TYPE);
//}
//
//
//
