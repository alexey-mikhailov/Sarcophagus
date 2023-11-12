#include "pch.h"
#include "FileMenuCommands.h"

#include "SarcophagusCommon.h"

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

		if (StorageFile pickedFile = co_await filePicker.PickSingleFileAsync())
		{
			if (Streams::IRandomAccessStream stream = co_await pickedFile.OpenAsync(FileAccessMode::Read))
			{
				// TODO Encrypt buffer, copy from mem to disk

				const winrt::Windows::Storage::Streams::DataReader reader(stream.GetInputStreamAt(0));

				if (co_await reader.LoadAsync(~0u))
				{
					const Sarcophagus::MainVM vm = MainVM::GetInstance();
					vm.Credentials().Clear();

					const uint32_t credentialCount = reader.ReadUInt32();

					for (uint32_t i = 0; i < credentialCount; ++i)
					{
						const uint32_t nameInBytes = reader.ReadUInt32();
						SARCOPHAGUS_ASSERT(nameInBytes % 2 == 0, NULL, L"Number of bytes of cred.key should be even number. ");

						const wchar_t* name = new wchar_t[nameInBytes / 2];
						array_view nameBuffer{ (uint8_t*)name, nameInBytes };

						reader.ReadBytes(nameBuffer);
						winrt::hstring credentialName(name, nameInBytes / 2);
						delete[] name;

						const uint32_t passwordInBytes = reader.ReadUInt32();
						SARCOPHAGUS_ASSERT(passwordInBytes % 2 == 0, NULL, L"Number of bytes of cred.value should be even number. ");

						const wchar_t* password = new wchar_t[passwordInBytes / 2];
						array_view passwordBuffer{ (uint8_t*)password, passwordInBytes };

						reader.ReadBytes(passwordBuffer);
						winrt::hstring credentialPassword(password, passwordInBytes / 2);
						delete[] password;

						vm.Credentials().Append(Sarcophagus::Credential(credentialName, credentialPassword));
					}
				}

				if (co_await stream.FlushAsync())
				{
					reader.Close();
					stream.Close();
				}
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

			if (StorageFile pickedFile = co_await filePicker.PickSaveFileAsync())
			{
				if (Streams::IRandomAccessStream stream = co_await pickedFile.OpenAsync(FileAccessMode::ReadWrite))
				{
					// TODO Encrypt buffer, copy from mem to disk

					const winrt::Sarcophagus::MainVM vm = MainVM::GetInstance();
					const winrt::Windows::Storage::Streams::DataWriter writer(stream.GetOutputStreamAt(0));

					const uint32_t credentialCount = vm.Credentials().Size();
					writer.WriteUInt32(credentialCount);

					for (auto credential : vm.Credentials())
					{
						const winrt::hstring name = credential.Name();
						const uint32_t nameInBytes = name.size() * 2;

						writer.WriteUInt32(nameInBytes);
						writer.WriteBytes({ (uint8_t*)name.c_str(), nameInBytes });

						const winrt::hstring password = credential.Password();
						const uint32_t passwordInBytes = password.size() * 2;

						writer.WriteUInt32(passwordInBytes);
						writer.WriteBytes({ (uint8_t*)password.c_str(), passwordInBytes });
					}

					if (co_await writer.StoreAsync())
					{
						if (co_await stream.FlushAsync())
						{
							writer.Close();
							stream.Close();
						}
					}
				}
			}
		}
	}
}