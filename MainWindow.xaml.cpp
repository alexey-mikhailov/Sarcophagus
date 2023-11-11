// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"

#include "CredEditor.xaml.h"
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
		}

		// Subscribe
		_vm.PageChanged(PageChangedDelegate(this, &MainWindow::OnPageChanged));

		// And invoke immediately (init)
		OnPageChanged(PageId::Main);
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
					page->Init(_vm);
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
					page->Init(_vm);
				}
			}
		}
	}
}
