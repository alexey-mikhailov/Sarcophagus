#pragma once
#include "CredFolder.g.h"
#include "Credential.g.h"
#include "MainVM.g.h"

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Sarcophagus::implementation
{
	struct CredFolder : CredFolderT<CredFolder>
	{
		CredFolder();
		CredFolder(winrt::hstring name);

		winrt::Sarcophagus::MainVM MainVM() const;
		IObservableVector<Sarcophagus::Credential> Credentials() const { return _credentials; }

		winrt::hstring Name() const { return _name; }
		void Name(const winrt::hstring& name);

		winrt::event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(const winrt::event_token& token);

	private:
		IObservableVector<Sarcophagus::Credential> _credentials;
		winrt::hstring _name;
		winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
	};

	struct Credential : CredentialT<Credential>
	{
		Credential() = default;
		Credential(winrt::Sarcophagus::CredFolder credFolder, 
		           winrt::hstring name, 
		           winrt::hstring login, 
		           winrt::hstring password) : _credFolder(credFolder),
		                                      _name(name),
		                                      _login(login),
		                                      _password(password) {}

		winrt::Sarcophagus::MainVM MainVM() const;
		winrt::Sarcophagus::CredFolder CredFolder() { return _credFolder; }

		winrt::hstring Name() const { return _name; }
		void Name(const winrt::hstring& name);

		winrt::hstring Login() const { return _login; }
		void Login(const winrt::hstring& login);

		winrt::hstring Password() const { return _password; }
		void Password(const winrt::hstring& password);

		winrt::event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(const winrt::event_token& token);

	private:
		winrt::Sarcophagus::CredFolder _credFolder;
		winrt::hstring _name;
		winrt::hstring _login;
		winrt::hstring _password;
		winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
	};

	struct MainVM : MainVMT<MainVM>
	{
		MainVM();

		Sarcophagus::PageId PageId() { return _pageId; }
		void PageId(winrt::Sarcophagus::PageId pageId);

		Sarcophagus::CredFolder SelectedCredFolder() const { return _selectedCredFolder; }
		void SelectedCredFolder(Sarcophagus::CredFolder const& value);
		IObservableVector<Sarcophagus::CredFolder> CredFolders() const { return _credFolders; }

		Sarcophagus::Credential CredentialTemplate() const { return _credentialTemplate; }
		void CredentialTemplate(Sarcophagus::Credential const& value);

		winrt::event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(const winrt::event_token& token);

		winrt::event_token PageChanged(PageChangedDelegate const& value) { return _pageChanged.add(value); }
		void PageChanged(winrt::event_token const& token) noexcept { _pageChanged.remove(token); }

		Sarcophagus::AddCredFolderCommand AddCredFolderCommand() const { return _addCredFolderCommand; }
		Sarcophagus::RemoveCredFolderCommand RemoveCredFolderCommand() const { return _removeCredFolderCommand; }
		Sarcophagus::EditCredFolderCommand EditCredFolderCommand() const { return _editCredFolderCommand; }
		Sarcophagus::AddCredentialCommand AddCredentialCommand() const { return _addCredentialCommand; }
		Sarcophagus::RemoveCredentialCommand RemoveCredentialCommand() const { return _removeCredentialCommand; }
		Sarcophagus::EditCredentialCommand EditCredentialCommand() const { return _editCredentialCommand; }
		Sarcophagus::CopyCredentialCommand CopyCredentialCommand() const { return _copyCredentialCommand; }
		Sarcophagus::ChooseCryptoengineToCreateFileCommand ChooseCryptoengineToCreateFileCommand() const { return _chooseCryptoengineToCreateFileCommand; }
		Sarcophagus::NewFileCommand NewFileCommand() const { return _newFileCommand; }
		Sarcophagus::OpenFileCommand OpenFileCommand() const { return _openFileCommand; }
		Sarcophagus::ChooseCryptoengineToSaveFileCommand ChooseCryptoengineToSaveFileCommand() const { return _chooseCryptoengineToSaveFileCommand; }
		Sarcophagus::SaveFileCommand SaveFileCommand() const { return _saveFileCommand; }
		Sarcophagus::ChooseCryptoengineToSaveFileAsCommand ChooseCryptoengineToSaveFileAsCommand() const { return _chooseCryptoengineToSaveFileAsCommand; }
		Sarcophagus::SaveFileAsCommand SaveFileAsCommand() const { return _saveFileAsCommand; }

	private:
		Sarcophagus::PageId _pageId = Sarcophagus::PageId::RecentFiles;
		Sarcophagus::CredFolder _selectedCredFolder = nullptr;
		IObservableVector<Sarcophagus::CredFolder> _credFolders;
		Sarcophagus::Credential _credentialTemplate = nullptr;

		Sarcophagus::AddCredFolderCommand _addCredFolderCommand;
		Sarcophagus::RemoveCredFolderCommand _removeCredFolderCommand;
		Sarcophagus::EditCredFolderCommand _editCredFolderCommand;
		Sarcophagus::AddCredentialCommand _addCredentialCommand;
		Sarcophagus::RemoveCredentialCommand _removeCredentialCommand;
		Sarcophagus::EditCredentialCommand _editCredentialCommand;
		Sarcophagus::CopyCredentialCommand _copyCredentialCommand;
		Sarcophagus::ChooseCryptoengineToCreateFileCommand _chooseCryptoengineToCreateFileCommand;
		Sarcophagus::NewFileCommand _newFileCommand;
		Sarcophagus::OpenFileCommand _openFileCommand;
		Sarcophagus::ChooseCryptoengineToSaveFileCommand _chooseCryptoengineToSaveFileCommand;
		Sarcophagus::SaveFileCommand _saveFileCommand;
		Sarcophagus::ChooseCryptoengineToSaveFileAsCommand _chooseCryptoengineToSaveFileAsCommand;
		Sarcophagus::SaveFileAsCommand _saveFileAsCommand;

		winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
		winrt::event<PageChangedDelegate> _pageChanged;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct CredFolder : CredFolderT<CredFolder, implementation::CredFolder> {};
	struct Credential : CredentialT<Credential, implementation::Credential> {};
	struct MainVM :     MainVMT<MainVM, implementation::MainVM> {};
}
