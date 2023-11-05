#pragma once

#define nameof(x) (#x)
#define wnameof(x) (L#x)

namespace Sarcophagus
{
	HWND GetWindowHandle(winrt::Microsoft::UI::Xaml::Window window);
	void Assert(bool expression, int32_t reason, const wchar_t* reasonStr, const wchar_t* message, const char* file, int line);
	void Fail(int32_t reason, const wchar_t* reasonStr, const wchar_t* message, const char* file, int line);
}

#define SARCOPHAGUS_ASSERT(bExpression, i32reason, wstrMessage) ::Sarcophagus::Assert(bExpression, i32reason, L#i32reason, wstrMessage, __FILE__, __LINE__)
#define SARCOPHAGUS_FAIL(i32reason, wstrMessage)                ::Sarcophagus::Fail(i32reason, L#i32reason, wstrMessage, __FILE__, __LINE__)
