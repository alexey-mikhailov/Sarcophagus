// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "RecentFilesPage.g.h"

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Sarcophagus::implementation
{
    struct RecentFilesPage : RecentFilesPageT<RecentFilesPage>
    {
        RecentFilesPage();
        void Init();
        IAsyncAction InitAsync();

        RecentFilesVM RecentFilesVM() const { return _vm; };

    private:
        Sarcophagus::RecentFilesVM _vm{ nullptr };
    };
}

namespace winrt::Sarcophagus::factory_implementation
{
    struct RecentFilesPage : RecentFilesPageT<RecentFilesPage, implementation::RecentFilesPage> {};
}
