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

	void CredEditor::OnOk(IInspectable const&, RoutedEventArgs const&)
	{
		if (pwdBox1().Password() == pwdBox2().Password())
		{
			// TODO: Encrypt

			winrt::hstring const& pwd = pwdBox1().Password();
			winrt::Sarcophagus::Credential const& tmp = _vm.CredentialTemplate();
			tmp.Password(pwd);
			_vm.PageId(Sarcophagus::PageId::Main);
		}
		else
		{
			Controls::ContentDialog dialog{};
			Controls::TextBlock textBlock{};
			textBlock.Text(L"Passwords do not match. ");
			dialog.Content(textBlock);
			dialog.CloseButtonText(L"OK");
			dialog.XamlRoot(XamlRoot());
			dialog.ShowAsync();
		}
	}

	void CredEditor::OnCancel(IInspectable const&, RoutedEventArgs const&)
	{
		_vm.PageId(Sarcophagus::PageId::Main);
	}
}
