// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"

#include "ChooseCryptoenginePage.xaml.h"
#include "CredFolderEditor.xaml.h"
#include "RecentFilesPage.xaml.h"
#include "CredEditor.xaml.h"
#include "DialogTools.h"
#include "MainPage.xaml.h"
#include "resource.h"

#include "SarcophagusCommon.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif


using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Sarcophagus::implementation
{
	MainWindow::MainWindow()
	{
		InitializeComponent();

		if (HWND hWnd = ::Sarcophagus::GetWindowHandle(*this))
		{
			HMODULE hModule;
			if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, ::Sarcophagus::EmptyString, &hModule))
			{
				HICON hIcon;        // icon handle 
				HRSRC hResource;    // handle to FindResource  
				HGLOBAL hMem;       // handle to LoadResource 
				LPVOID lpResource;  // pointer to resource data  
				int nID;            // ID of resource that best fits current screen 

				// Find the icon directory whose identifier is 440. 
				hResource = FindResource(hModule, MAKEINTRESOURCE(IDI_MAIN), RT_GROUP_ICON);

				// Load and lock the icon directory. 
				hMem = LoadResource(hModule, hResource);
				lpResource = LockResource(hMem);

				// Get the identifier of the icon that is most appropriate 
				// for the video display. 

				nID = LookupIconIdFromDirectoryEx((PBYTE)lpResource, TRUE, 0, 0, LR_DEFAULTCOLOR);

				// Find the bits for the nID icon. 
				hResource = FindResource(hModule, MAKEINTRESOURCE(nID), RT_ICON);

				// Load and lock the icon. 
				hMem = LoadResource(hModule, hResource);

				lpResource = LockResource(hMem);

				// Create a handle to the icon. 
				hIcon = CreateIconFromResourceEx((PBYTE)lpResource, SizeofResource(hModule, hResource), TRUE, 0x00030000, 0, 0, LR_DEFAULTCOLOR);

				SendMessage(hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
			}

			_vm.NewFileCommand().WindowHandle(reinterpret_cast<int64_t>(hWnd));
			_vm.OpenFileCommand().WindowHandle(reinterpret_cast<int64_t>(hWnd));
			_vm.SaveFileCommand().WindowHandle(reinterpret_cast<int64_t>(hWnd));
			_vm.SaveFileAsCommand().WindowHandle(reinterpret_cast<int64_t>(hWnd));
		}

		// Subscribe
		_vm.PageChanged(PageChangedDelegate(this, &MainWindow::OnPageChanged));

		// And invoke immediately (init)
		OnPageChanged(PageId::RecentFiles);
	}

	void MainWindow::OnContentLoaded(IInspectable const&, RoutedEventArgs const&)
	{
		SetTitleBar(AppTitleBar());
		ExtendsContentIntoTitleBar(true);
	}

	void MainWindow::OnPageChanged(Sarcophagus::PageId pageId)
	{
		if (pageId == PageId::Main)
		{
			// Change breadcrumb bar content. 
			BreadcrumbBar().ItemsSource(winrt::single_threaded_observable_vector<hstring>({ L"Main" }));

			// Navigate to new frame
			if (HostedPageFrame().Navigate(xaml_typename<Sarcophagus::MainPage>()))
			{
				if (const auto page = HostedPageFrame().Content().try_as<Sarcophagus::implementation::MainPage>())
				{
					page->Init();
				}
			}
		}
		else if (pageId == PageId::RecentFiles)
		{
			// Change breadcrumb bar content. 
			BreadcrumbBar().ItemsSource(winrt::single_threaded_observable_vector<hstring>({ L"Main", L"Recent files" }));

			// Navigate to new frame
			if (HostedPageFrame().Navigate(xaml_typename<Sarcophagus::RecentFilesPage>()))
			{
				if (const auto page = HostedPageFrame().Content().try_as<Sarcophagus::implementation::RecentFilesPage>())
				{
					page->Init();
				}
			}
		}
		else if (pageId == PageId::EditCredFolder)
		{
			// Change breadcrumb bar content. 
			BreadcrumbBar().ItemsSource(winrt::single_threaded_observable_vector<hstring>({ L"Main", L"Edit folder"}));

			// Navigate to new frame
			if (HostedPageFrame().Navigate(xaml_typename<Sarcophagus::CredFolderEditor>()))
			{
				if (const auto page = HostedPageFrame().Content().try_as<Sarcophagus::implementation::CredFolderEditor>())
				{
					page->Init();
				}
			}
		}
		else if (pageId == PageId::EditCredential)
		{
			// Change breadcrumb bar content. 
			BreadcrumbBar().ItemsSource(winrt::single_threaded_observable_vector<hstring>({ L"Main", L"Edit credential"}));

			// Navigate to new frame
			if (HostedPageFrame().Navigate(xaml_typename<Sarcophagus::CredEditor>()))
			{
				if (const auto page = HostedPageFrame().Content().try_as<Sarcophagus::implementation::CredEditor>())
				{
					page->Init();
				}
			}
		}
		else if (pageId == PageId::ChooseCryptoengine)
		{
			// Change breadcrumb bar content. 
			BreadcrumbBar().ItemsSource(winrt::single_threaded_observable_vector<hstring>({ L"Main", L"Choose crypto engine"}));

			// Navigate to new frame
			if (HostedPageFrame().Navigate(xaml_typename<Sarcophagus::ChooseCryptoenginePage>()))
			{
				if (const auto page = HostedPageFrame().Content().try_as<Sarcophagus::implementation::ChooseCryptoenginePage>())
				{
					page->Init();
				}
			}
		}
	}

	IAsyncAction MainWindow::OnClosed(IInspectable const&, WindowEventArgs const& args)
	{
		if (FileSerializer::GetInstance().IsDirty())
		{
			args.Handled(true);
			bool isSaveNeeded = false;
			bool isCloseNeeded = false;

			auto decisionBox = ::Sarcophagus::MakeDecisionBox(L"Do you want to save this file?");
			decisionBox.Button1Handler([&isSaveNeeded](ContentDialog, ContentDialogButtonClickEventArgs)
			{
				isSaveNeeded = true;
			});
			decisionBox.Button2Handler([&isCloseNeeded](ContentDialog, ContentDialogButtonClickEventArgs)
			{
				isCloseNeeded = true;
			});

			co_await decisionBox.ShowAsync();

			if (isSaveNeeded)
			{
				co_await ::Sarcophagus::ViewModelHub::GetInstance().MainVM().SaveFileCommand().ExecuteAsync();
				Close();
			}
			else if (isCloseNeeded)
			{
				FileSerializer::GetInstance().ClearDirty();
				Close();
			}
			else
			{
				// Async case. 
				// Cancel button. Do nothing. 
			}
		}
		else
		{
			// Immediate case. 
			// File wasn't dirty. Just close app. 
			args.Handled(false);
		}
	}
}

