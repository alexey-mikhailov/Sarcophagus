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

		Sarcophagus::Credential CredentialTemplate() const { return _credentialTemplate; }
		IObservableVector<Sarcophagus::Credential> Credentials() const { return _credentials; }

	private:
		Sarcophagus::Credential _credentialTemplate = nullptr;
		IObservableVector<Sarcophagus::Credential> _credentials;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct Credential : CredentialT<Credential, implementation::Credential>	{};
	struct MainVM :     MainVMT<MainVM, implementation::MainVM> {};
}
