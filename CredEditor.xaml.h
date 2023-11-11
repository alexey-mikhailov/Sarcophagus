// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "CredEditor.g.h"

namespace winrt::Sarcophagus::implementation
{
	struct CredEditor : CredEditorT<CredEditor>
	{
		CredEditor();
		void Init(Sarcophagus::MainVM const& vm);
		void myButton_Click(Windows::Foundation::IInspectable const& sender,
		                    Microsoft::UI::Xaml::RoutedEventArgs const& args);

		Sarcophagus::MainVM MainVM() const { return _vm; }

	private:
		Sarcophagus::MainVM _vm{nullptr};
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct CredEditor : CredEditorT<CredEditor, implementation::CredEditor>
	{
	};
}
