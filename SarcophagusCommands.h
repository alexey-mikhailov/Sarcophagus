#pragma once
#include "EditCredentialCommand.g.h"
#include "CopyCredentialCommand.g.h"

using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
    struct EditCredentialCommand : EditCredentialCommandT<EditCredentialCommand>
    {
        EditCredentialCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

    private:
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
	};

	struct CopyCredentialCommand : CopyCredentialCommandT<CopyCredentialCommand>
    {
        CopyCredentialCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

    private:
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
    struct EditCredentialCommand : EditCredentialCommandT<EditCredentialCommand, implementation::EditCredentialCommand> {};
    struct CopyCredentialCommand : CopyCredentialCommandT<CopyCredentialCommand, implementation::CopyCredentialCommand> {};
}
