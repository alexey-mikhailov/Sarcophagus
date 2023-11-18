// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"

#include "cryptoselector.h"
#include "DialogTools.h"
#include "InternalCryptoTool.h"
#include "MainWindow.xaml.h"
#include "SarcophagusCommon.h"
#include "Serialization/FileSerializer.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Navigation;
using namespace winrt::Sarcophagus::implementation;
using namespace ::Sarcophagus;

App* App::_instance;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
	_instance = this;
	InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
	UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
	{
		if (IsDebuggerPresent())
		{
			auto errorMessage = e.Message();
			__debugbreak();
		}
	});
#endif
}

/// <summary>
/// Invoked when the application is launched.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const&)
{
	_window = make<MainWindow>();

	// Initialize serializer.
	FileSerializer::GetInstance().Initialize();

	// Initialize internal crypto tool.
	InternalCryptoTool::GetInstance().Setup(0x100, CipherTable);

	// Scan cryptoengines. 
	ViewModelHub::GetInstance().ChooseCryptoengineVM().Rescan();

	// Set window size
	if (HWND hWnd = GetWindowHandle(_window))
	{
		// Retrieve the WindowId that corresponds to hWnd.
		winrt::Microsoft::UI::WindowId windowId = Microsoft::UI::GetWindowIdFromWindow(hWnd);
		winrt::Microsoft::UI::Windowing::AppWindow appWindow = winrt::Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);

		if (appWindow)
		{
			appWindow.Resize({ 600, 300 });
		}
	}

	_window.ExtendsContentIntoTitleBar(true);
	_window.Activate();
}
