// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once
#include "MainWindow.g.h"

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
	struct MainWindow : MainWindowT<MainWindow>
	{
		MainWindow();
		void OnContentLoaded(IInspectable const& sender, RoutedEventArgs const& args);
		void OnPageChanged(Sarcophagus::PageId pageId);

		Sarcophagus::MainVM MainVM() const { return _vm; }
		Sarcophagus::FileSerializer FileSerializer() const { return _fileSerializer; }

	private:
		Sarcophagus::FileSerializer _fileSerializer{FileSerializer::GetInstance()};
		Sarcophagus::MainVM _vm{MainVM::GetInstance()};
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
	{
	};
}
