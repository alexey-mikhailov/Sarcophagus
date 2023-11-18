#pragma once
#include "AddCredentialCommand.g.h"
#include "RemoveCredentialCommand.g.h"
#include "EditCredentialCommand.g.h"
#include "CopyCredentialCommand.g.h"
#include "ChooseCryptoengineCommand.g.h"

using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
    struct AddCredentialCommand : AddCredentialCommandT<AddCredentialCommand>
    {
        AddCredentialCommand() = default;
        winrt::event_token CanExecuteChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

    private:
        winrt::event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> _canExecuteChanged;
	};

    struct RemoveCredentialCommand : RemoveCredentialCommandT<RemoveCredentialCommand>
    {
        RemoveCredentialCommand() = default;
        winrt::event_token CanExecuteChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

    private:
        winrt::event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> _canExecuteChanged;
	};

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

	struct ChooseCryptoengineCommand : ChooseCryptoengineCommandT<ChooseCryptoengineCommand>
    {
        ChooseCryptoengineCommand() = default;
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
    struct AddCredentialCommand : AddCredentialCommandT<AddCredentialCommand, implementation::AddCredentialCommand> {};
    struct RemoveCredentialCommand : RemoveCredentialCommandT<RemoveCredentialCommand, implementation::RemoveCredentialCommand> {};
    struct EditCredentialCommand : EditCredentialCommandT<EditCredentialCommand, implementation::EditCredentialCommand> {};
    struct CopyCredentialCommand : CopyCredentialCommandT<CopyCredentialCommand, implementation::CopyCredentialCommand> {};
    struct ChooseCryptoengineCommand : ChooseCryptoengineCommandT<ChooseCryptoengineCommand, implementation::ChooseCryptoengineCommand> {};
}
