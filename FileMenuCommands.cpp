#include "pch.h"
#include "FileMenuCommands.h"

#include "App.xaml.h"
#include "InternalCryptoTool.h"
#include "SarcophagusCommon.h"
#include "Serialization/FileSerializer.h"

#if __has_include("NewFileCommand.g.cpp")
#include "NewFileCommand.g.cpp"
#endif
#if __has_include("OpenFileCommand.g.cpp")
#include "OpenFileCommand.g.cpp"
#endif
#if __has_include("SaveFileCommand.g.cpp")
#include "SaveFileCommand.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;

namespace winrt::Sarcophagus::implementation
{
	// TODO: NewFileCommand: Choose encryption plugin.

	void NewFileCommand::Execute(IInspectable const&)
	{
		ExecuteAsync();
	}

	IAsyncAction NewFileCommand::ExecuteAsync()
	{
		if (FileSerializer::GetInstance().IsDirty())
		{
			auto mainWindow = App::GetInstance().Window();

			Controls::ContentDialog dialog{};
			Controls::TextBlock textBlock{};
			textBlock.Text(L"Do you want to save this file?");
			textBlock.VerticalAlignment(VerticalAlignment::Center);
			dialog.Content(textBlock);
			dialog.PrimaryButtonText(L"Yes");
			dialog.PrimaryButtonClick([mainWindow](Controls::ContentDialog, Controls::ContentDialogButtonClickEventArgs)
			{
				MainVM::GetInstance().SaveFileCommand().Execute(mainWindow);
			});
			dialog.SecondaryButtonText(L"No");
			dialog.SecondaryButtonClick([mainWindow](Controls::ContentDialog, Controls::ContentDialogButtonClickEventArgs)
			{
				MainVM::GetInstance().Credentials().Clear();
				FileSerializer::GetInstance().ClearDirty();
			});

			dialog.CloseButtonText(L"Cancel");
			dialog.XamlRoot(mainWindow.Content().XamlRoot());
			co_await dialog.ShowAsync();
		}
		else
		{
			MainVM::GetInstance().Credentials().Clear();
			FileSerializer::GetInstance().ClearDirty();
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
			Controls::ContentDialog dialog{};
			Controls::TextBlock textBlock{};
			textBlock.Text(L"Do you want to save this file?");
			textBlock.VerticalAlignment(VerticalAlignment::Center);
			dialog.Content(textBlock);
			dialog.PrimaryButtonText(L"Yes");
			dialog.PrimaryButtonClick([&isSaveNeeded, mainWindow](Controls::ContentDialog, Controls::ContentDialogButtonClickEventArgs)
			{
				isSaveNeeded = true;
			});
			dialog.SecondaryButtonText(L"No");
			dialog.CloseButtonText(L"Cancel");
			dialog.CloseButtonClick([&isOpenNeeded, mainWindow](Controls::ContentDialog, Controls::ContentDialogButtonClickEventArgs)
			{
				isOpenNeeded = false;
			});

			dialog.XamlRoot(mainWindow.Content().XamlRoot());
			co_await dialog.ShowAsync();
		}

		if (isSaveNeeded)
		{
			MainVM::GetInstance().SaveFileCommand().Execute(mainWindow);
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
				winrt::hstring password;
				Controls::ContentDialog enterPwdDialog{};
				Controls::StackPanel stackPanel{};
				Controls::TextBlock textBlockPassword{};
				Controls::Border space{};
				Controls::PasswordBox textBoxPassword{};
				stackPanel.Orientation(Controls::Orientation::Horizontal);
				stackPanel.Children().Append(textBlockPassword);
				stackPanel.Children().Append(space);
				stackPanel.Children().Append(textBoxPassword);
				textBlockPassword.Text(L"Password");
				textBlockPassword.VerticalAlignment(VerticalAlignment::Center);
				space.Width(5.0);
				textBoxPassword.PlaceholderText(L"Enter file password");
				textBoxPassword.Width(250.0);
				textBoxPassword.Height(30.0);
				enterPwdDialog.Content(stackPanel);
				enterPwdDialog.PrimaryButtonText(L"OK");
				enterPwdDialog.PrimaryButtonClick([&password, textBoxPassword](Controls::ContentDialog, Controls::ContentDialogButtonClickEventArgs)
				{
					password = textBoxPassword.Password();
				});
				enterPwdDialog.CloseButtonText(L"Close");
				enterPwdDialog.XamlRoot(mainWindow.Content().XamlRoot());
				co_await enterPwdDialog.ShowAsync();

				if (password.empty())
				{
					Controls::ContentDialog dialog{};
					Controls::TextBlock textBlock{};
					textBlock.Text(L"Empty password is not allowed. ");
					dialog.Content(textBlock);
					dialog.CloseButtonText(L"OK");
					dialog.XamlRoot(mainWindow.Content().XamlRoot());
					co_await dialog.ShowAsync();
				}
				else
				{
					uint64_t pwdSize;
					uint8_t* pwdBuff;
					const ::Sarcophagus::InternalCryptoTool::EncryptResult result = ::Sarcophagus::InternalCryptoTool::GetInstance().Encrypt
					(
						sizeof(winrt::hstring::value_type) * textBoxPassword.Password().size(),
						reinterpret_cast<const uint8_t*>(textBoxPassword.Password().c_str()),
						&pwdSize,
						&pwdBuff
					);

					SARCOPHAGUS_ASSERT(result == ::Sarcophagus::InternalCryptoTool::EncryptResult::Success, NULL, L"Internal encryption failed. ");
					SARCOPHAGUS_ASSERT(pwdSize % 2 == 0, NULL, L"Enrypted password should be wide string (with even size). ");

					FileSerializer::GetInstance().SetStorageKey(pwdSize, reinterpret_cast<uint64_t>(pwdBuff));
					delete pwdBuff;

					co_await FileSerializer::GetInstance().OpenFileAsync(pickedFile);
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
		auto mainWindow = App::GetInstance().Window();

		winrt::hstring password;
		Controls::ContentDialog enterPwdDialog{};
		Controls::StackPanel stackPanel{};
		Controls::TextBlock textBlockPassword{};
		Controls::Border space{};
		Controls::PasswordBox textBoxPassword{};
		stackPanel.Orientation(Controls::Orientation::Horizontal);
		stackPanel.Children().Append(textBlockPassword);
		stackPanel.Children().Append(space);
		stackPanel.Children().Append(textBoxPassword);
		textBlockPassword.Text(L"Password");
		textBlockPassword.VerticalAlignment(VerticalAlignment::Center);
		space.Width(5.0);
		textBoxPassword.PlaceholderText(L"Enter file password");
		textBoxPassword.Width(250.0);
		textBoxPassword.Height(30.0);
		enterPwdDialog.Content(stackPanel);
		enterPwdDialog.PrimaryButtonText(L"OK");
		enterPwdDialog.PrimaryButtonClick([&password, textBoxPassword](Controls::ContentDialog, Controls::ContentDialogButtonClickEventArgs)
		{
			password = textBoxPassword.Password();
		});
		enterPwdDialog.CloseButtonText(L"Close");
		enterPwdDialog.XamlRoot(mainWindow.Content().XamlRoot());
		co_await enterPwdDialog.ShowAsync();

		if (password.empty())
		{
			Controls::ContentDialog dialog{};
			Controls::TextBlock textBlock{};
			textBlock.Text(L"Empty password is not allowed. ");
			dialog.Content(textBlock);
			dialog.CloseButtonText(L"OK");
			dialog.XamlRoot(mainWindow.Content().XamlRoot());
			co_await dialog.ShowAsync();
		}
		else
		{
			uint64_t pwdSize;
			uint8_t* pwdBuff;
			const ::Sarcophagus::InternalCryptoTool::EncryptResult result = ::Sarcophagus::InternalCryptoTool::GetInstance().Encrypt
			(
				sizeof(winrt::hstring::value_type) * textBoxPassword.Password().size(),
				reinterpret_cast<const uint8_t*>(textBoxPassword.Password().c_str()),
				&pwdSize,
				&pwdBuff
			);

			SARCOPHAGUS_ASSERT(result == ::Sarcophagus::InternalCryptoTool::EncryptResult::Success, NULL, L"Internal encryption failed. ");
			SARCOPHAGUS_ASSERT(pwdSize % 2 == 0, NULL, L"Enrypted password should be wide string (with even size). ");

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
				}
			}
		}
	}
}