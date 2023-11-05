#include "pch.h"
#include "SarcophagusCommon.h"

namespace Sarcophagus
{
	HWND GetWindowHandle(winrt::Microsoft::UI::Xaml::Window window)
	{
		if (const auto windowNative = window.try_as<IWindowNative>())
		{
			HWND hWnd;
			if (SUCCEEDED(windowNative->get_WindowHandle(&hWnd)))
			{
				return hWnd;
			}
		}

		return NULL;
	}

	void Assert(bool expression, int32_t reason, const wchar_t* reasonStr, const wchar_t* message, const char* file, int line)
	{
		if (!expression)
		{
			std::wstringstream ss;
			ss << "Assert failed with error code " << reasonStr << "(0x" << std::hex << reason << ") at " << std::endl << std::endl
			   << file << " : line " << std::dec << line << std::endl << std::endl
			   << "MSG: " << message << std::endl;

			MessageBoxW(0, ss.str().c_str(), L"Assert", MB_OK | MB_ICONEXCLAMATION);
			throw winrt::hresult_error(reason, winrt::hstring(ss.str().c_str()));
		}
	}

	void Fail(int32_t reason, const wchar_t* reasonStr, const wchar_t* message, const char* file, int line)
	{
		std::wstringstream ss;
		ss << "Assert failed with error code " << reasonStr << "(0x" << std::hex << reason << ") at " << std::endl << std::endl
		   << file << " : line " << std::dec << line << std::endl << std::endl
		   << "MSG: " << message << std::endl;

		MessageBoxW(0, ss.str().c_str(), L"Assert", MB_OK | MB_ICONEXCLAMATION);
		throw winrt::hresult_error(reason, winrt::hstring(ss.str().c_str()));
	}
}
