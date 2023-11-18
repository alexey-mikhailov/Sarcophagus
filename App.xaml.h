// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"

using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::Sarcophagus::implementation
{
	struct App : AppT<App>
	{
		static App& GetInstance() { return *_instance; }

		App();
		void OnLaunched(LaunchActivatedEventArgs const&);
		winrt::Sarcophagus::MainWindow Window() const { return _window; }

	private:
		static App* _instance;
		winrt::Sarcophagus::MainWindow _window{ nullptr };
	};
}
