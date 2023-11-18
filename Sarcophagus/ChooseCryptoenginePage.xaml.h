#pragma once
#include "ChooseCryptoenginePage.g.h"

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Sarcophagus::implementation
{
	struct ChooseCryptoenginePage : ChooseCryptoenginePageT<ChooseCryptoenginePage>
    {
        ChooseCryptoenginePage();
        void Init();
        void OnCancel(IInspectable const& sender, RoutedEventArgs const& e);

        Sarcophagus::ChooseCryptoengineVM ChooseCryptoengineVM() const { return _vm; }

    private:
        Sarcophagus::ChooseCryptoengineVM _vm{ nullptr };
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct ChooseCryptoenginePage : ChooseCryptoenginePageT<ChooseCryptoenginePage, implementation::ChooseCryptoenginePage> {};
}
