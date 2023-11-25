#include "pch.h"
#include "MainVM.h"

#include "SarcophagusCommon.h"
#include "ViewModelHub.h"

#if __has_include("CredFolder.g.cpp")
#include "CredFolder.g.cpp"
#endif
#if __has_include("Credential.g.cpp")
#include "Credential.g.cpp"
#endif
#if __has_include("MainVM.g.cpp")
#include "MainVM.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::Sarcophagus::implementation
{
	CredFolder::CredFolder()
	{
		_credentials = winrt::single_threaded_observable_vector<Sarcophagus::Credential>();
	}

	CredFolder::CredFolder(winrt::hstring name): _name(name)
	{
		_credentials = winrt::single_threaded_observable_vector<Sarcophagus::Credential>();
	}

	winrt::Sarcophagus::MainVM CredFolder::MainVM() const
	{
		return ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
	}

	void CredFolder::Name(const winrt::hstring& name)
	{
		if (name != _name)
		{
			_name = name;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(Name)));
		}
	}

	winrt::event_token CredFolder::PropertyChanged(PropertyChangedEventHandler const& value)
	{
		return _propertyChanged.add(value);
	}

	void CredFolder::PropertyChanged(const winrt::event_token& token)
	{
		_propertyChanged.remove(token);
	}

	winrt::Sarcophagus::MainVM Credential::MainVM() const
	{
		return ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
	}

	void Credential::Name(const winrt::hstring& name)
	{
		if (name != _name)
		{
			_name = name;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(Name)));
		}
	}

	void Credential::Login(const winrt::hstring& login)
	{
		if (login != _login)
		{
			_login = login;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(Login)));
		}
	}

	void Credential::Password(const winrt::hstring& password)
	{
		if (password != _password)
		{
			_password = password;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(Password)));
		}
	}

	winrt::event_token Credential::PropertyChanged(PropertyChangedEventHandler const& value)
	{
		return _propertyChanged.add(value);
	}

	void Credential::PropertyChanged(const winrt::event_token& token)
	{
		_propertyChanged.remove(token);
	}

	MainVM::MainVM()
	{
		_credFolders = winrt::single_threaded_observable_vector<Sarcophagus::CredFolder>();
	}

	void MainVM::PageId(winrt::Sarcophagus::PageId pageId)
	{
		if (pageId != _pageId)
		{
			_pageChanged(pageId);
		}

		_pageId = pageId;
	}

	void MainVM::SelectedCredFolder(Sarcophagus::CredFolder const& value)
	{
		if (value != _selectedCredFolder)
		{
			_selectedCredFolder = value;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(SelectedCredFolder)));
		}
	}

	void MainVM::CredentialTemplate(Sarcophagus::Credential const& value)
	{
		if (value != _credentialTemplate)
		{
			_credentialTemplate = value;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(CredentialTemplate)));
		}
	}

	winrt::event_token MainVM::PropertyChanged(PropertyChangedEventHandler const& value)
	{
		return _propertyChanged.add(value);
	}

	void MainVM::PropertyChanged(const winrt::event_token& token)
	{
		_propertyChanged.remove(token);
	}
}
