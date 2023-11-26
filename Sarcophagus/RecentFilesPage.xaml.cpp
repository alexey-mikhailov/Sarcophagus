// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "RecentFilesPage.xaml.h"

#include "ViewModelHub.h"

#if __has_include("RecentFilesPage.g.cpp")
#include "RecentFilesPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Sarcophagus::implementation
{
    RecentFilesPage::RecentFilesPage()
    {
        InitializeComponent();
    }

    void RecentFilesPage::Init()
    {
        _vm = ::Sarcophagus::ViewModelHub::GetInstance().RecentFilesVM();
        InitAsync();
    }

    IAsyncAction RecentFilesPage::InitAsync()
    {
        co_return;
    }
}
