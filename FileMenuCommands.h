#pragma once
#include "OpenFileCommand.g.h"
#include "SaveFileCommand.g.h"

using namespace winrt::Windows::Foundation;

namespace winrt::Sarcophagus::implementation
{
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

    struct SaveFileCommand : SaveFileCommandT<SaveFileCommand>
    {
        SaveFileCommand() = default;
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
}

namespace winrt::Sarcophagus::factory_implementation
{
    struct OpenFileCommand : OpenFileCommandT<OpenFileCommand, implementation::OpenFileCommand> {};
    struct SaveFileCommand : SaveFileCommandT<SaveFileCommand, implementation::SaveFileCommand> {};
}