#pragma once
#include "Credential.g.h"
#include "MainVM.g.h"

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Sarcophagus::implementation
{
	struct Credential : CredentialT<Credential>
	{
		Credential() = default;
		Credential(winrt::hstring name, winrt::hstring password) : _name(name), _password(password) {}

		winrt::Sarcophagus::MainVM MainVM() const;

		winrt::hstring Name() const { return _name; }
		void Name(const winrt::hstring& name);

		winrt::hstring Password() const { return _password; }
		void Password(const winrt::hstring& password);

		winrt::event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(const winrt::event_token& token);

	private:
		winrt::hstring _name;
		winrt::hstring _password;
		winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
	};

	struct MainVM : MainVMT<MainVM>
	{
		MainVM();

		Sarcophagus::PageId PageId() { return _pageId; }
		void PageId(winrt::Sarcophagus::PageId pageId);

		Sarcophagus::Credential CredentialTemplate() const { return _credentialTemplate; }
		void CredentialTemplate(Sarcophagus::Credential const& value) { _credentialTemplate = value; }
		IObservableVector<Sarcophagus::Credential> Credentials() const { return _credentials; }

		winrt::event_token PageChanged(PageChangedDelegate const& value) { return _pageChanged.add(value); }
		void PageChanged(winrt::event_token const& token) noexcept { _pageChanged.remove(token); }

		Sarcophagus::AddCredentialCommand AddCredentialCommand() const { return _addCredentialCommand; }
		Sarcophagus::RemoveCredentialCommand RemoveCredentialCommand() const { return _removeCredentialCommand; }
		Sarcophagus::EditCredentialCommand EditCredentialCommand() const { return _editCredentialCommand; }
		Sarcophagus::CopyCredentialCommand CopyCredentialCommand() const { return _copyCredentialCommand; }
		Sarcophagus::ChooseCryptoengineToCreateFileCommand ChooseCryptoengineToCreateFileCommand() const { return _chooseCryptoengineToCreateFileCommand; }
		Sarcophagus::NewFileCommand NewFileCommand() const { return _newFileCommand; }
		Sarcophagus::OpenFileCommand OpenFileCommand() const { return _openFileCommand; }
		Sarcophagus::ChooseCryptoengineToSaveFileCommand ChooseCryptoengineToSaveFileCommand() const { return _chooseCryptoengineToSaveFileCommand; }
		Sarcophagus::SaveFileCommand SaveFileCommand() const { return _saveFileCommand; }

	private:
		Sarcophagus::PageId _pageId = Sarcophagus::PageId::ChooseCryptoengine;
		Sarcophagus::Credential _credentialTemplate = nullptr;
		IObservableVector<Sarcophagus::Credential> _credentials;

		Sarcophagus::AddCredentialCommand _addCredentialCommand;
		Sarcophagus::RemoveCredentialCommand _removeCredentialCommand;
		Sarcophagus::EditCredentialCommand _editCredentialCommand;
		Sarcophagus::CopyCredentialCommand _copyCredentialCommand;
		Sarcophagus::ChooseCryptoengineToCreateFileCommand _chooseCryptoengineToCreateFileCommand;
		Sarcophagus::NewFileCommand _newFileCommand;
		Sarcophagus::OpenFileCommand _openFileCommand;
		Sarcophagus::ChooseCryptoengineToSaveFileCommand _chooseCryptoengineToSaveFileCommand;
		Sarcophagus::SaveFileCommand _saveFileCommand;

		winrt::event<PageChangedDelegate> _pageChanged;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct Credential : CredentialT<Credential, implementation::Credential>	{};
	struct MainVM :     MainVMT<MainVM, implementation::MainVM> {};
}
