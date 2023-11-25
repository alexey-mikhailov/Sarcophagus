#pragma once
#include "ChooseCryptoengineToCreateFileCommand.g.h"
#include "NewFileCommand.g.h"
#include "OpenFileCommand.g.h"
#include "ChooseCryptoengineToSaveFileCommand.g.h"
#include "SaveFileCommand.g.h"
#include "ChooseCryptoengineToSaveFileAsCommand.g.h"
#include "SaveFileAsCommand.g.h"

using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
    struct ChooseCryptoengineToCreateFileCommand : ChooseCryptoengineToCreateFileCommandT<ChooseCryptoengineToCreateFileCommand>
    {
        ChooseCryptoengineToCreateFileCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

    private:
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
    };

    struct NewFileCommand : NewFileCommandT<NewFileCommand>
    {
        NewFileCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

        int64_t WindowHandle() const { return (int64_t)_hWnd; }
        void WindowHandle(int64_t value) { _hWnd = (HWND)value; }

    private:
        IAsyncAction ExecuteAsync();
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
        HWND _hWnd;
    };

    struct OpenFileCommand : OpenFileCommandT<OpenFileCommand>
    {
        OpenFileCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

        int64_t WindowHandle() const { return (int64_t)_hWnd; }
        void WindowHandle(int64_t value) { _hWnd = (HWND)value; }

    private:
        IAsyncAction ExecuteAsync();
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
        HWND _hWnd;
    };

    struct ChooseCryptoengineToSaveFileCommand : ChooseCryptoengineToSaveFileCommandT<ChooseCryptoengineToSaveFileCommand>
    {
        ChooseCryptoengineToSaveFileCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

    private:
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
    };

    struct SaveFileCommand : SaveFileCommandT<SaveFileCommand>
    {
        SaveFileCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);
        IAsyncAction ExecuteAsync();

        int64_t WindowHandle() const { return (int64_t)_hWnd; }
        void WindowHandle(int64_t value) { _hWnd = (HWND)value; }

    private:
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
        HWND _hWnd;
    };

    struct ChooseCryptoengineToSaveFileAsCommand : ChooseCryptoengineToSaveFileAsCommandT<ChooseCryptoengineToSaveFileAsCommand>
    {
        ChooseCryptoengineToSaveFileAsCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);

    private:
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
    };

    struct SaveFileAsCommand : SaveFileAsCommandT<SaveFileAsCommand>
    {
        SaveFileAsCommand() = default;
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& value) { return _canExecuteChanged.add(value); }
        void CanExecuteChanged(winrt::event_token const& token) { _canExecuteChanged.remove(token); }
        auto CanExecute(IInspectable const&) { return true; }
        void Execute(IInspectable const& parameter);
        IAsyncAction ExecuteAsync();

        int64_t WindowHandle() const { return (int64_t)_hWnd; }
        void WindowHandle(int64_t value) { _hWnd = (HWND)value; }

    private:
        winrt::event<EventHandler<IInspectable>> _canExecuteChanged;
        HWND _hWnd;
    };
}

namespace winrt::Sarcophagus::factory_implementation
{
    struct ChooseCryptoengineToCreateFileCommand : ChooseCryptoengineToCreateFileCommandT<ChooseCryptoengineToCreateFileCommand, implementation::ChooseCryptoengineToCreateFileCommand> {};
    struct NewFileCommand : NewFileCommandT<NewFileCommand, implementation::NewFileCommand> {};
    struct OpenFileCommand : OpenFileCommandT<OpenFileCommand, implementation::OpenFileCommand> {};
    struct ChooseCryptoengineToSaveFileCommand : ChooseCryptoengineToSaveFileCommandT<ChooseCryptoengineToSaveFileCommand, implementation::ChooseCryptoengineToSaveFileCommand> {};
    struct SaveFileCommand : SaveFileCommandT<SaveFileCommand, implementation::SaveFileCommand> {};
    struct ChooseCryptoengineToSaveFileAsCommand : ChooseCryptoengineToSaveFileCommandT<ChooseCryptoengineToSaveFileAsCommand, implementation::ChooseCryptoengineToSaveFileAsCommand> {};
    struct SaveFileAsCommand : SaveFileAsCommandT<SaveFileAsCommand, implementation::SaveFileAsCommand> {};
}
