#pragma once
#include "DecisionBox.g.h"
#include "PasswordDecisionBox.g.h"
#include "SarcophagusCommon.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;

namespace winrt::Sarcophagus::implementation
{
	struct DecisionBox;
}

namespace Sarcophagus
{
	IAsyncAction ShowErrorAsync(winrt::hstring const& text);
	winrt::Sarcophagus::DecisionBox MakeDecisionBox(winrt::hstring const& text);
	winrt::Sarcophagus::PasswordDecisionBox MakePasswordDecisionBox(winrt::hstring const& text,
	                                                                winrt::hstring const& placeholderText);
}

namespace winrt::Sarcophagus::implementation
{
	struct DecisionBox : DecisionBoxT<DecisionBox>
	{
		DecisionBox() = default;
		DecisionBox(winrt::hstring const& text) : _text(text) {}

		IAsyncAction ShowAsync();

		winrt::hstring Text() const { return _text; }
		void Text(winrt::hstring const& text) { _text = text; }
		winrt::hstring Button1Text() const { return _button1Text; }
		void Button1Text(winrt::hstring const& text) { _button1Text = text; }
		winrt::hstring Button2Text() const { return _button2Text; }
		void Button2Text(winrt::hstring const& text) { _button2Text = text; }
		winrt::hstring ButtonCloseText() const { return _buttonCloseText; }
		void ButtonCloseText(winrt::hstring const& text) { _buttonCloseText = text; }
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> Button1Handler() const { return _button1Handler; }
		void Button1Handler(TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> const& value) { _button1Handler = value; }
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> Button2Handler() const { return _button2Handler; }
		void Button2Handler(TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> const& value) { _button2Handler = value; }
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> ButtonCloseHandler() const { return _buttonCloseHandler; }
		void ButtonCloseHandler(TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> const& value) { _buttonCloseHandler = value; }

	private:
		winrt::hstring _text;
		winrt::hstring _button1Text = L"Yes";
		winrt::hstring _button2Text = L"No";
		winrt::hstring _buttonCloseText = L"Cancel";
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> _button1Handler = ::Sarcophagus::EmptyTypedEventHandler;
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> _button2Handler = ::Sarcophagus::EmptyTypedEventHandler;
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> _buttonCloseHandler = ::Sarcophagus::EmptyTypedEventHandler;
	};

	struct PasswordDecisionBox : PasswordDecisionBoxT<PasswordDecisionBox>
	{
		PasswordDecisionBox() = default;
		PasswordDecisionBox(winrt::hstring const& text, 
		                    winrt::hstring const& placeholderText)
			: _text(text)
			, _placeholderText(placeholderText)
		{}
		IAsyncOperation<PasswordDecisionResult> ShowAsync();

		winrt::hstring Text() const { return _text; }
		void Text(winrt::hstring const& text) { _text = text; }
		winrt::hstring PlaceholderText() const { return _placeholderText; }
		void PlaceholderText(winrt::hstring const& text) { _placeholderText = text; }
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> ButtonOkHandler() const { return _buttonOkHandler; }
		void ButtonOkHandler(TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> const& value) { _buttonOkHandler = value; }
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> ButtonCancelHandler() const { return _buttonCancelHandler; }
		void ButtonCancelHandler(TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> const& value) { _buttonCancelHandler = value; }

	private:
		winrt::hstring _text = L"Password";
		winrt::hstring _placeholderText;
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> _buttonOkHandler = ::Sarcophagus::EmptyTypedEventHandler;
		TypedEventHandler<ContentDialog, ContentDialogButtonClickEventArgs> _buttonCancelHandler = ::Sarcophagus::EmptyTypedEventHandler;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct DecisionBox : DecisionBoxT<DecisionBox, implementation::DecisionBox> {};
	struct PasswordDecisionBox : PasswordDecisionBoxT<PasswordDecisionBox, implementation::PasswordDecisionBox> {};
}
