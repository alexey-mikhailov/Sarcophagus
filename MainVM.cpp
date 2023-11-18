#include "pch.h"
#include "MainVM.h"

#include "SarcophagusCommon.h"
#include "ViewModelHub.h"

#if __has_include("Credential.g.cpp")
#include "Credential.g.cpp"
#endif
#if __has_include("MainVM.g.cpp")
#include "MainVM.g.cpp"
#endif

namespace winrt::Sarcophagus::implementation
{
	winrt::Sarcophagus::MainVM Credential::MainVM() const
	{
		return ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
	}

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
	}

	void MainVM::PageId(winrt::Sarcophagus::PageId pageId)
	{
		if (pageId != _pageId)
		{
			_pageChanged(pageId);
		}

		_pageId = pageId;
	}
}
