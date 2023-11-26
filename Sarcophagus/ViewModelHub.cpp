#include "pch.h"
#include "ViewModelHub.h"
#include "ChooseCryptoengineVM.h"
#include "RecentFilesVM.h"

namespace Sarcophagus
{
	ViewModelHub::ViewModelHub()
		: _mainVM(winrt::make<winrt::Sarcophagus::implementation::MainVM>())
		, _chooseCryptoengineVM(winrt::make<winrt::Sarcophagus::implementation::ChooseCryptoengineVM>())
		, _recentFilesVM(winrt::make<winrt::Sarcophagus::implementation::RecentFilesVM>())
	{
	}
}