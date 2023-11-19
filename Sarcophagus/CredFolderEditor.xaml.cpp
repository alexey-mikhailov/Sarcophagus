// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "CredFolderEditor.xaml.h"
#if __has_include("CredFolderEditor.g.cpp")
#include "CredFolderEditor.g.cpp"
#endif

#include "DialogTools.h"
#include "ViewModelHub.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Sarcophagus::implementation
{
    CredFolderEditor::CredFolderEditor()
    {
        InitializeComponent();
    }

	void CredFolderEditor::Init()
	{
		_vm = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
		if (_vm.SelectedCredFolder())
		{
			_oldName = _vm.SelectedCredFolder().Name();
		}
		else
		{
			::Sarcophagus::ShowErrorAsync(L"Expected valid selected credential folder. ");
		}
	}

	void CredFolderEditor::OnOk(IInspectable const&, RoutedEventArgs const&)
	{
		bool isDirty = false;

		winrt::Sarcophagus::CredFolder const& selectedCredFolder = _vm.SelectedCredFolder();
		if (selectedCredFolder)
		{
			_vm.PageId(Sarcophagus::PageId::Main);

			if (_oldName != selectedCredFolder.Name())
			{
				isDirty = true;
			}

			if (isDirty)
			{
				FileSerializer::GetInstance().MakeDirty();
			}
		}
		else
		{
			::Sarcophagus::ShowErrorAsync(L"Expected valid selected credential folder. ");
		}
	}

	void CredFolderEditor::OnCancel(IInspectable const&, RoutedEventArgs const&)
	{
		_vm.PageId(Sarcophagus::PageId::Main);
	}
}
