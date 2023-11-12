// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "CredEditor.xaml.h"
#if __has_include("CredEditor.g.cpp")
#include "CredEditor.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Sarcophagus::implementation
{
	CredEditor::CredEditor()
	{
		InitializeComponent();
	}

	void CredEditor::Init(Sarcophagus::MainVM const& vm)
	{
		_vm = vm;
	}

	void CredEditor::OnBack(IInspectable const&, RoutedEventArgs const&)
	{
		_vm.PageId(Sarcophagus::PageId::Main);
	}
}
