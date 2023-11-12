#include "pch.h"
#include "FileMenuCommands.h"

#if __has_include("OpenFileCommand.g.cpp")
#include "OpenFileCommand.g.cpp"
#endif
#if __has_include("SaveFileCommand.g.cpp")
#include "SaveFileCommand.g.cpp"
#endif

using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;

namespace winrt::Sarcophagus::implementation
{
	void OpenFileCommand::Execute(IInspectable const&)
	{
		ExecuteAsync();
	}

	IAsyncAction OpenFileCommand::ExecuteAsync()
	{
		auto filePicker = FileOpenPicker();
		filePicker.SuggestedStartLocation(Pickers::PickerLocationId::ComputerFolder);
		filePicker.FileTypeFilter().Append(L".bin");
		auto initializeWithWindow{ filePicker.as<::IInitializeWithWindow>() };

		initializeWithWindow->Initialize(_hWnd);
		StorageFile pickedFile = co_await filePicker.PickSingleFileAsync();

		if (pickedFile)
		{
			if (auto stream = pickedFile.OpenAsync(FileAccessMode::Read))
			{
				// TODO: Deserialize file
			}
		}
	}

	void SaveFileCommand::Execute(IInspectable const&)
	{
		ExecuteAsync();
	}

	IAsyncAction SaveFileCommand::ExecuteAsync()
	{
		wchar_t fileName[MAX_PATH];
		if (GetModuleFileNameW(NULL, fileName, MAX_PATH))
		{
			auto file = co_await StorageFile::GetFileFromPathAsync(fileName);
			auto folder = co_await file.GetParentAsync();
			auto localFolder = folder.Path();

			auto filePicker = FileSavePicker();
			filePicker.SuggestedStartLocation(Pickers::PickerLocationId::ComputerFolder);
			auto fullPath = localFolder + L"\\data.bin";
			filePicker.SuggestedFileName(fullPath);
			auto extensions = winrt::single_threaded_vector<hstring>();
			extensions.Append(L".bin");
			filePicker.FileTypeChoices().Insert(L"sarcophagus binary file", extensions);

			auto initializeWithWindow{ filePicker.as<::IInitializeWithWindow>() };
			initializeWithWindow->Initialize(_hWnd);

			StorageFile pickedFile = co_await filePicker.PickSaveFileAsync();

			if (pickedFile)
			{
				if (auto stream = pickedFile.OpenAsync(FileAccessMode::ReadWrite))
				{
					// TODO: Serialize file
				}
			}
		}
	}

}