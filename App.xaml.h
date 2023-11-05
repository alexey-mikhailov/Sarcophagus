// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"

using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::Sarcophagus::implementation
{
	struct App : AppT<App>
	{
		App();

		void OnLaunched(LaunchActivatedEventArgs const&);

	private:
		Window window{ nullptr };
	};
}
