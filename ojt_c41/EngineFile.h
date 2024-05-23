#pragma once

namespace helper
{
	struct DialogPath
	{
		std::wstring path;
	};

	DialogPath OpenDialog();
	DialogPath SaveDialog();

	class FileWriter
	{
	public:
		FileWriter(const DialogPath& path);
		virtual ~FileWriter();
		FileWriter(const FileWriter&) = delete;
		FileWriter& operator=(const FileWriter&) = delete;

		void Write(const void* const buffer, const UINT elementSize);
		void WriteLine(const std::wstring& str);


		FILE* mFile;
	};
}