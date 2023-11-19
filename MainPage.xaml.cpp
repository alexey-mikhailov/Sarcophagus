// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainPage.xaml.h"
#include "MainWindow.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
	MainPage::MainPage()
	{
		InitializeComponent();
	}

	void MainPage::Init()
	{
		_vm = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
	}

	void MainPage::OnCredFolderSelected(IInspectable const&, 
	                                    SelectionChangedEventArgs const&)
	{
		if (_vm.SelectedCredFolder())
		{
			// Complex binding path worked in WPF, now I have to force it via code behind.
			// There still seems to be WinUI bug.
			listCredentials().ItemsSource(_vm.SelectedCredFolder().Credentials());
		}
	}
}
