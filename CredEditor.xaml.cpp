// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "CredEditor.xaml.h"
#if __has_include("CredEditor.g.cpp")
#include "CredEditor.g.cpp"
#endif

#include "DialogTools.h"
#include "InternalCryptoTool.h"
#include "SarcophagusCommon.h"
#include "ViewModelHub.h"

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

	void CredEditor::Init()
	{
		_vm = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
		_oldName = _vm.CredentialTemplate().Name();
	}

	void CredEditor::OnOk(IInspectable const&, RoutedEventArgs const&)
	{
		bool isDirty = false;

		if (pwdBox1().Password() == pwdBox2().Password())
		{
			uint64_t pwdSize;
			uint8_t* pwdBuff;
			const ::Sarcophagus::InternalCryptoTool::EncryptResult result = ::Sarcophagus::InternalCryptoTool::GetInstance().Encrypt
			(
				sizeof(winrt::hstring::value_type) * pwdBox1().Password().size(),
				reinterpret_cast<const uint8_t*>(pwdBox1().Password().c_str()),
				&pwdSize,
				&pwdBuff
			);

			SARCOPHAGUS_ASSERT(result == ::Sarcophagus::InternalCryptoTool::EncryptResult::Success, NULL, L"Internal encryption failed. ");
			SARCOPHAGUS_ASSERT(pwdSize % 2 == 0, NULL, L"Encrypted password should be wide string (with even size). ");

			const winrt::hstring pwd
			{
				reinterpret_cast<wchar_t*>(pwdBuff),
				static_cast<hstring::size_type>(pwdSize / 2)
			};

			winrt::Sarcophagus::Credential const& tmp = _vm.CredentialTemplate();

			if (tmp.Password() != pwd)
			{
				isDirty = true;
			}

			tmp.Password(pwd);
			_vm.PageId(Sarcophagus::PageId::Main);
		}
		else
		{
			::Sarcophagus::ShowErrorAsync(L"Passwords do not match. ");
			return;
		}

		if (_oldName != _vm.CredentialTemplate().Name())
		{
			isDirty = true;
		}

		if (isDirty)
		{
			FileSerializer::GetInstance().MakeDirty();
		}
	}

	void CredEditor::OnCancel(IInspectable const&, RoutedEventArgs const&)
	{
		_vm.PageId(Sarcophagus::PageId::Main);
	}
}
