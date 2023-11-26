#include "pch.h"
#include "FileMenuCommands.h"

#include "App.xaml.h"
#include "DialogTools.h"
#include "InternalCryptoTool.h"
#include "SarcophagusCommon.h"
#include "ViewModelHub.h"
#include "Serialization/FileSerializer.h"

#if __has_include("ChooseCryptoengineToCreateFileCommand.g.cpp")
#include "ChooseCryptoengineToCreateFileCommand.g.cpp"
#endif
#if __has_include("NewFileCommand.g.cpp")
#include "NewFileCommand.g.cpp"
#endif
#if __has_include("OpenFileCommand.g.cpp")
#include "OpenFileCommand.g.cpp"
#endif
#if __has_include("ChooseCryptoengineToSaveFileCommand.g.cpp")
#include "ChooseCryptoengineToSaveFileCommand.g.cpp"
#endif
#if __has_include("SaveFileCommand.g.cpp")
#include "SaveFileCommand.g.cpp"
#endif
#if __has_include("ChooseCryptoengineToSaveFileAsCommand.g.cpp")
#include "ChooseCryptoengineToSaveFileAsCommand.g.cpp"
#endif
#if __has_include("SaveFileAsCommand.g.cpp")
#include "SaveFileAsCommand.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;

namespace winrt::Sarcophagus::implementation
{
	void ChooseCryptoengineToCreateFileCommand::Execute(IInspectable const&)
	{
		winrt::Sarcophagus::ChooseCryptoengineVM chooseCryptoengineVM = ::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM();
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
		chooseCryptoengineVM.Target(ChooseCryptoengineTarget::NewFile);
		mainVM.PageId(Sarcophagus::PageId::ChooseCryptoengine);
	}

	void NewFileCommand::Execute(IInspectable const&)
	{
		ExecuteAsync();
	}

	IAsyncAction NewFileCommand::ExecuteAsync()
	{
		if (FileSerializer::GetInstance().IsDirty())
		{
			bool isSaveNeeded = false;
			bool isCancelNeeded = false;

			auto decisionBox = ::Sarcophagus::MakeDecisionBox(L"Do you want to save this file?");
			decisionBox.Button1Handler([&isSaveNeeded](ContentDialog, ContentDialogButtonClickEventArgs)
			{
				isSaveNeeded = true;
			});
			decisionBox.Button2Handler([&isCancelNeeded](ContentDialog, ContentDialogButtonClickEventArgs)
			{
				isCancelNeeded = true;
			});

			co_await decisionBox.ShowAsync();

			if (isSaveNeeded)
			{
				::Sarcophagus::ViewModelHub::GetInstance().MainVM().SaveFileCommand().Execute(nullptr);
			}
			else if (isCancelNeeded)
			{
				::Sarcophagus::ViewModelHub::GetInstance().MainVM().CredFolders().Clear();
				FileSerializer::GetInstance().ClearDirty();
			}
		}
		else
		{
			Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
			mainVM.CredFolders().Clear();
			FileSerializer::GetInstance().ClearDirty();
			FileSerializer::GetInstance().FileToSave(nullptr);
			mainVM.ChooseCryptoengineToCreateFileCommand().Execute(nullptr);
		}
	}

	void OpenFileCommand::Execute(IInspectable const&)
	{
		ExecuteAsync();
	}

	IAsyncAction OpenFileCommand::ExecuteAsync()
	{
		bool isOpenNeeded = true;
		bool isSaveNeeded = false;
		auto mainWindow = App::GetInstance().Window();

		if (FileSerializer::GetInstance().IsDirty())
		{
			auto decision = ::Sarcophagus::MakeDecisionBox(L"Do you want to save this file?");
			decision.Button1Handler([&isSaveNeeded](ContentDialog, ContentDialogButtonClickEventArgs)
			{
				isSaveNeeded = true;
			});
			decision.ButtonCloseHandler([&isOpenNeeded](ContentDialog, ContentDialogButtonClickEventArgs)
			{
				isOpenNeeded = false;
			});

			co_await decision.ShowAsync();
		}

		if (isSaveNeeded)
		{
			::Sarcophagus::ViewModelHub::GetInstance().MainVM().SaveFileCommand().Execute(mainWindow);
		}
		else if (isOpenNeeded)
		{
			auto filePicker = FileOpenPicker();
			filePicker.SuggestedStartLocation(Pickers::PickerLocationId::ComputerFolder);
			filePicker.FileTypeFilter().Append(L".bin");
			auto initializeWithWindow{ filePicker.as<::IInitializeWithWindow>() };

			initializeWithWindow->Initialize(_hWnd);

			if (StorageFile pickedFile = co_await filePicker.PickSingleFileAsync())
			{
				auto decisionResult = co_await ::Sarcophagus::MakePasswordDecisionBox(L"Password", L"Enter file password").ShowAsync();
				if (decisionResult.Status == PasswordDecisionStatus::Ok)
				{
					if (decisionResult.Password.empty())
					{
						co_await ::Sarcophagus::ShowErrorAsync(L"Empty password is not allowed. ");
					}
					else
					{
						uint64_t pwdSize;
						uint8_t* pwdBuff;
						const ::Sarcophagus::InternalCryptoTool::EncryptResult result = ::Sarcophagus::InternalCryptoTool::GetInstance().Encrypt
						(
							sizeof(winrt::hstring::value_type) * decisionResult.Password.size(),
							reinterpret_cast<const uint8_t*>(decisionResult.Password.c_str()),
							&pwdSize,
							&pwdBuff
						);

						SARCOPHAGUS_ASSERT(result == ::Sarcophagus::InternalCryptoTool::EncryptResult::Success, NULL, L"Internal encryption failed. ");
						SARCOPHAGUS_ASSERT(pwdSize % 2 == 0, NULL, L"Encrypted password should be wide string (with even size). ");

						FileSerializer::GetInstance().SetStorageKey(pwdSize, reinterpret_cast<uint64_t>(pwdBuff));
						delete pwdBuff;

						co_await FileSerializer::GetInstance().OpenFileAsync(pickedFile);
						FileSerializer::GetInstance().ClearDirty();
						FileSerializer::GetInstance().FileToSave(pickedFile);
						::Sarcophagus::ViewModelHub::GetInstance().RecentFilesVM().PushRecentFile(pickedFile);
						::Sarcophagus::ViewModelHub::GetInstance().MainVM().PageId(PageId::Main);
					}
				}
			}
		}
	}

	void ChooseCryptoengineToSaveFileCommand::Execute(IInspectable const&)
	{
		if (FileSerializer::GetInstance().FileToSave())
		{
			Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
			mainVM.SaveFileCommand().Execute(nullptr);
		}
		else
		{
			winrt::Sarcophagus::ChooseCryptoengineVM chooseCryptoengineVM = ::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM();
			Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
			chooseCryptoengineVM.Target(ChooseCryptoengineTarget::SaveFile);
			mainVM.PageId(Sarcophagus::PageId::ChooseCryptoengine);
		}
	}

	void SaveFileCommand::Execute(IInspectable const&)
	{
		ExecuteAsync();
	}

	IAsyncAction SaveFileCommand::ExecuteAsync()
	{
		auto decisionResult = co_await ::Sarcophagus::MakePasswordDecisionBox(L"Password", L"Enter file password").ShowAsync();
		if (decisionResult.Status == PasswordDecisionStatus::Ok)
		{
			if (decisionResult.Password.empty())
			{
				co_await ::Sarcophagus::ShowErrorAsync(L"Empty password is not allowed. ");
			}
			else
			{
				uint64_t pwdSize;
				uint8_t* pwdBuff;
				const ::Sarcophagus::InternalCryptoTool::EncryptResult result = ::Sarcophagus::InternalCryptoTool::GetInstance().Encrypt
				(
					sizeof(winrt::hstring::value_type) * decisionResult.Password.size(),
					reinterpret_cast<const uint8_t*>(decisionResult.Password.c_str()),
					&pwdSize,
					&pwdBuff
				);

				SARCOPHAGUS_ASSERT(result == ::Sarcophagus::InternalCryptoTool::EncryptResult::Success, NULL, L"Internal encryption failed. ");
				SARCOPHAGUS_ASSERT(pwdSize % 2 == 0, NULL, L"Encrypted password should be wide string (with even size). ");

				FileSerializer::GetInstance().SetStorageKey(pwdSize, reinterpret_cast<uint64_t>(pwdBuff));
				delete pwdBuff;

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

					StorageFile pickedFile = FileSerializer::GetInstance().FileToSave() ? FileSerializer::GetInstance().FileToSave() : co_await filePicker.PickSaveFileAsync();

					if (pickedFile)
					{
						co_await FileSerializer::GetInstance().SaveFileAsync(pickedFile);
						FileSerializer::GetInstance().ClearDirty();
						FileSerializer::GetInstance().FileToSave(pickedFile);
						::Sarcophagus::ViewModelHub::GetInstance().RecentFilesVM().PushRecentFile(pickedFile);
						::Sarcophagus::ViewModelHub::GetInstance().MainVM().PageId(PageId::Main);
					}
				}
			}
		}
	}

	void ChooseCryptoengineToSaveFileAsCommand::Execute(IInspectable const&)
	{
		winrt::Sarcophagus::ChooseCryptoengineVM chooseCryptoengineVM = ::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM();
		Sarcophagus::MainVM mainVM = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
		chooseCryptoengineVM.Target(ChooseCryptoengineTarget::SaveFileAs);
		mainVM.PageId(Sarcophagus::PageId::ChooseCryptoengine);
	}

	void SaveFileAsCommand::Execute(IInspectable const&)
	{
		ExecuteAsync();
	}

	IAsyncAction SaveFileAsCommand::ExecuteAsync()
	{
		auto decisionResult = co_await ::Sarcophagus::MakePasswordDecisionBox(L"Password", L"Enter file password").ShowAsync();
		if (decisionResult.Status == PasswordDecisionStatus::Ok)
		{
			if (decisionResult.Password.empty())
			{
				co_await ::Sarcophagus::ShowErrorAsync(L"Empty password is not allowed. ");
			}
			else
			{
				uint64_t pwdSize;
				uint8_t* pwdBuff;
				const ::Sarcophagus::InternalCryptoTool::EncryptResult result = ::Sarcophagus::InternalCryptoTool::GetInstance().Encrypt
				(
					sizeof(winrt::hstring::value_type) * decisionResult.Password.size(),
					reinterpret_cast<const uint8_t*>(decisionResult.Password.c_str()),
					&pwdSize,
					&pwdBuff
				);

				SARCOPHAGUS_ASSERT(result == ::Sarcophagus::InternalCryptoTool::EncryptResult::Success, NULL, L"Internal encryption failed. ");
				SARCOPHAGUS_ASSERT(pwdSize % 2 == 0, NULL, L"Encrypted password should be wide string (with even size). ");

				FileSerializer::GetInstance().SetStorageKey(pwdSize, reinterpret_cast<uint64_t>(pwdBuff));
				delete pwdBuff;

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
						co_await FileSerializer::GetInstance().SaveFileAsync(pickedFile);
						FileSerializer::GetInstance().ClearDirty();
						FileSerializer::GetInstance().FileToSave(pickedFile);
						::Sarcophagus::ViewModelHub::GetInstance().RecentFilesVM().PushRecentFile(pickedFile);
						::Sarcophagus::ViewModelHub::GetInstance().MainVM().PageId(PageId::Main);
					}
				}
			}
		}
	}
}