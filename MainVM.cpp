#include "pch.h"
#include "MainVM.h"
#include "Credential.g.cpp"
#include "MainVM.g.cpp"
#include <SarcophagusCommon.h>

namespace winrt::Sarcophagus::implementation
{
	void Credential::Name(const winrt::hstring& name)
	{
		if (name != _name)
		{
			_name = name;
			_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(wnameof(Name)));
		}
	}

	void Credential::Password(const winrt::hstring& password)
	{
		if (password != _password)
		{
			_password = password;
			_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(wnameof(Password)));
		}
	}

	winrt::event_token Credential::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value)
	{
		return _propertyChanged.add(value);
	}

	void Credential::PropertyChanged(const winrt::event_token& token)
	{
		_propertyChanged.remove(token);
	}

	MainVM::MainVM()
	{
		_credentials = winrt::single_threaded_observable_vector<Sarcophagus::Credential>();
		_credentials.Append(Sarcophagus::Credential(L"Key 1", L"Value 1"));
		_credentials.Append(Sarcophagus::Credential(L"Key 2", L"Value 2"));
		_credentials.Append(Sarcophagus::Credential(L"Key 3", L"Value 3"));
	}
}