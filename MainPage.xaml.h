// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainPage.g.h"

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
	struct MainPage : MainPageT<MainPage>
	{
		MainPage();
		void Init();

		Sarcophagus::MainVM MainVM() const { return _vm; }

	private:
		Sarcophagus::MainVM _vm { nullptr };
	public:
		void OnCredFolderSelected(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct MainPage : MainPageT<MainPage, implementation::MainPage>
	{
	};
}
