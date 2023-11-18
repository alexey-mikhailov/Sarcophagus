// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "ChooseCryptoenginePage.xaml.h"

#include "ViewModelHub.h"

#if __has_include("ChooseCryptoenginePage.g.cpp")
#include "ChooseCryptoenginePage.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Storage;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Sarcophagus::implementation
{
	ChooseCryptoenginePage::ChooseCryptoenginePage()
    {
        InitializeComponent();
    }

    void ChooseCryptoenginePage::Init()
    {
        _vm = ::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM();
        _vm.Rescan();
	}

	void ChooseCryptoenginePage::OnCancel(IInspectable const&, RoutedEventArgs const&)
    {
	    ::Sarcophagus::ViewModelHub::GetInstance().MainVM().PageId(PageId::Main);
    }
}




