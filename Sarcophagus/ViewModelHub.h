#pragma once
#include "MainVM.h"
#include "ChooseCryptoengineVM.h"

namespace Sarcophagus
{
	struct ViewModelHub final
	{
		static ViewModelHub& GetInstance()
		{
			static ViewModelHub instance;
			return instance;
		}

		winrt::Sarcophagus::MainVM MainVM() const { return _mainVM; }
		winrt::Sarcophagus::ChooseCryptoengineVM ChooseCryptoengineVM() const { return _chooseCryptoengineVM; }

	private:
		ViewModelHub();
		~ViewModelHub() = default;

		winrt::Sarcophagus::MainVM _mainVM;
		winrt::Sarcophagus::ChooseCryptoengineVM _chooseCryptoengineVM;
	};
}
