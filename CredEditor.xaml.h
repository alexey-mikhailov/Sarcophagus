#pragma once
#include "CredEditor.g.h"

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
	struct CredEditor : CredEditorT<CredEditor>
	{
		CredEditor();
		void Init();
		void OnOk(IInspectable const& sender, RoutedEventArgs const& e);
		void OnCancel(IInspectable const& sender, RoutedEventArgs const& e);
		void OnGeneratePassword(winrt::Windows::Foundation::IInspectable const& sender, RoutedEventArgs const& e);

		Sarcophagus::MainVM MainVM() const { return _vm; }

	private:
		winrt::hstring GeneratePassword() const;
		Sarcophagus::MainVM _vm{nullptr};
		winrt::hstring _oldName;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct CredEditor : CredEditorT<CredEditor, implementation::CredEditor>
	{
	};
}
