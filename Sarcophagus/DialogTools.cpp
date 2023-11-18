#include "pch.h"
#include "DialogTools.h"
#include "App.xaml.h"

#if __has_include("DecisionBox.g.cpp")
#include "DecisionBox.g.cpp"
#endif
#if __has_include("PasswordDecisionBox.g.cpp")
#include "PasswordDecisionBox.g.cpp"
#endif

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;

namespace Sarcophagus
{
	IAsyncAction ShowErrorAsync(winrt::hstring const& text)
	{
		auto mainWindow = winrt::Sarcophagus::implementation::App::GetInstance().Window();
		ContentDialog dialog{};
		TextBlock textBlock{};
		textBlock.Text(text);
		dialog.Content(textBlock);
		dialog.CloseButtonText(L"OK");
		dialog.DefaultButton(ContentDialogButton::Close);
		dialog.XamlRoot(mainWindow.Content().XamlRoot());
		co_await dialog.ShowAsync();
	}

	winrt::Sarcophagus::DecisionBox MakeDecisionBox(winrt::hstring const& text)
	{
		return winrt::make<winrt::Sarcophagus::implementation::DecisionBox>(text);
	}

	winrt::Sarcophagus::PasswordDecisionBox MakePasswordDecisionBox(winrt::hstring const& text,
	                                                                winrt::hstring const& placeholderText)
	{
		return winrt::make<winrt::Sarcophagus::implementation::PasswordDecisionBox>(text, placeholderText);
	}
}

namespace winrt::Sarcophagus::implementation
{
	IAsyncAction DecisionBox::ShowAsync()
	{
		auto mainWindow = App::GetInstance().Window();
		ContentDialog dialog{};
		TextBlock textBlock{};
		textBlock.Text(_text);
		textBlock.VerticalAlignment(VerticalAlignment::Center);
		dialog.Content(textBlock);
		dialog.PrimaryButtonText(_button1Text);
		dialog.PrimaryButtonClick(_button1Handler);
		dialog.SecondaryButtonText(_button2Text);
		dialog.SecondaryButtonClick(_button2Handler);

		dialog.CloseButtonText(_buttonCloseText);
		dialog.CloseButtonClick(_buttonCloseHandler);
		dialog.XamlRoot(mainWindow.Content().XamlRoot());
		co_await dialog.ShowAsync();
	}

	IAsyncOperation<PasswordDecisionResult> PasswordDecisionBox::ShowAsync()
	{
		auto mainWindow = App::GetInstance().Window();
		ContentDialog enterPwdDialog{};
		StackPanel stackPanel{};
		TextBlock textBlockPassword{};
		Border space{};
		PasswordBox textBoxPassword{};
		stackPanel.Orientation(Orientation::Horizontal);
		stackPanel.Children().Append(textBlockPassword);
		stackPanel.Children().Append(space);
		stackPanel.Children().Append(textBoxPassword);
		textBlockPassword.Text(_text);
		textBlockPassword.VerticalAlignment(VerticalAlignment::Center);
		space.Width(5.0);
		textBoxPassword.PlaceholderText(_placeholderText);
		textBoxPassword.Width(250.0);
		textBoxPassword.Height(30.0);
		enterPwdDialog.Content(stackPanel);
		enterPwdDialog.PrimaryButtonText(L"OK");
		enterPwdDialog.PrimaryButtonClick(_buttonOkHandler);
		enterPwdDialog.CloseButtonText(L"Cancel");
		enterPwdDialog.DefaultButton(ContentDialogButton::Primary);
		enterPwdDialog.XamlRoot(mainWindow.Content().XamlRoot());
		ContentDialogResult result = co_await enterPwdDialog.ShowAsync();

		if (result == ContentDialogResult::Primary)
		{
			co_return { PasswordDecisionStatus::Ok, textBoxPassword.Password() };
		}
		else
		{
			co_return { PasswordDecisionStatus::Cancel, ::Sarcophagus::EmptyString };
		}
	}
}