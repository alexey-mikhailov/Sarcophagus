#include "pch.h"
#include "ViewModelHub.h"

namespace Sarcophagus
{
	ViewModelHub::ViewModelHub()
	{
		_mainVM = winrt::make<winrt::Sarcophagus::implementation::MainVM>();
		_chooseCryptoengineVM = winrt::make<winrt::Sarcophagus::implementation::ChooseCryptoengineVM>();
	}
}