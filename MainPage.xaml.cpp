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

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Sarcophagus::implementation
{
	MainPage::MainPage()
	{
		InitializeComponent();
	}

	void MainPage::Init(Sarcophagus::MainVM const& vm)
	{
		_vm = vm;
	}

	void MainPage::OnEditCredential(IInspectable const&, RoutedEventArgs const&)
	{
		_vm.PageId(Sarcophagus::PageId::EditCredential);
	}

	void MainPage::OnCopyCredential(IInspectable const&, RoutedEventArgs const&)
	{
	}
}
