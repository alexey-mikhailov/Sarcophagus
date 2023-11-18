#pragma once
#include "CryptoengineVM.g.h"
#include "ChooseCryptoengineVM.g.h"

using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Sarcophagus::implementation
{
	struct CryptoengineVM : CryptoengineVMT<CryptoengineVM>
	{
		CryptoengineVM() = default;
		CryptoengineVM(winrt::hstring name, uint64_t uuidPart1, uint64_t uuidPart2)
			: _name(name)
			, _uuidPart1(uuidPart1)
			, _uuidPart2(uuidPart2)
		{}

		winrt::Sarcophagus::ChooseCryptoengineVM ChooseCryptoengineVM() const;

		winrt::hstring Name() const { return _name; }
		void Name(const winrt::hstring& name);
		uint64_t UuidPart1() const { return _uuidPart1; }
		uint64_t UuidPart2() const { return _uuidPart2; }

		winrt::event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(const winrt::event_token& token);

	private:
		winrt::hstring _name;
		uint64_t _uuidPart1;
		uint64_t _uuidPart2;
		winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
	};

	struct ChooseCryptoengineVM : ChooseCryptoengineVMT<ChooseCryptoengineVM>
	{
		ChooseCryptoengineVM();
		void Rescan();

		winrt::Sarcophagus::ChooseCryptoengineTarget Target() const { return _target; }
		void Target(winrt::Sarcophagus::ChooseCryptoengineTarget target) { _target = target; }
		IObservableVector<Sarcophagus::CryptoengineVM> Cryptoengines() const { return _cryptoengines; }
		Sarcophagus::ChooseCryptoengineCommand ChooseCryptoengineCommand() const { return _chooseCryptoengineCommand; }

	private:
		IAsyncAction RescanAsync();

		winrt::Sarcophagus::ChooseCryptoengineTarget _target;
		IObservableVector<Sarcophagus::CryptoengineVM> _cryptoengines;
		Sarcophagus::ChooseCryptoengineCommand _chooseCryptoengineCommand;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct CryptoengineVM : CryptoengineVMT<CryptoengineVM, implementation::CryptoengineVM> {};
	struct ChooseCryptoengineVM : ChooseCryptoengineVMT<ChooseCryptoengineVM, implementation::ChooseCryptoengineVM> {};
}
