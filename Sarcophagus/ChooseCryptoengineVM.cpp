#include "pch.h"
#include "ChooseCryptoengineVM.h"

#include "SarcophagusCommon.h"
#include "ViewModelHub.h"
#include "cryptoselector.h"
#include <functional>

#if __has_include("CryptoengineVM.g.cpp")
#include "CryptoengineVM.g.cpp"
#endif
#if __has_include("ChooseCryptoengineVM.g.cpp")
#include "ChooseCryptoengineVM.g.cpp"
#endif

namespace winrt::Sarcophagus::implementation
{
	winrt::Sarcophagus::ChooseCryptoengineVM CryptoengineVM::ChooseCryptoengineVM() const
	{
		return ::Sarcophagus::ViewModelHub::GetInstance().ChooseCryptoengineVM();
	}

	void CryptoengineVM::Name(const winrt::hstring& name)
	{
		if (name != _name)
		{
			_name = name;
			_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(wnameof(Name)));
		}
	}

	winrt::event_token CryptoengineVM::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value)
	{
		return _propertyChanged.add(value);
	}

	void CryptoengineVM::PropertyChanged(const winrt::event_token& token)
	{
		_propertyChanged.remove(token);
	}

	ChooseCryptoengineVM::ChooseCryptoengineVM()
	{
		_cryptoengines = winrt::single_threaded_observable_vector<Sarcophagus::CryptoengineVM>();
	}

	void ChooseCryptoengineVM::Rescan()
	{
		RescanAsync();
	}

	IAsyncAction ChooseCryptoengineVM::RescanAsync()
	{
		wchar_t moduleFileName[MAX_PATH];
		if (GetModuleFileNameW(NULL, moduleFileName, MAX_PATH))
		{
			auto moduleFile = co_await StorageFile::GetFileFromPathAsync(moduleFileName);
			auto folder = co_await moduleFile.GetParentAsync();
			auto files = co_await folder.GetFilesAsync();

			std::vector<std::wstring> dllPaths;
			for (const StorageFile& file : files)
			{
				const uint64_t filePathSize = file.Path().size();
				const wchar_t* filePath = file.Path().c_str();

				bool isDll = true;
				isDll &= filePath[filePathSize - 4] == L'.';
				isDll &= filePath[filePathSize - 3] == L'd';
				isDll &= filePath[filePathSize - 2] == L'l';
				isDll &= filePath[filePathSize - 1] == L'l';

				if (isDll)
				{
					dllPaths.emplace_back(filePath);
				}
			}

			srfg::cryptoselector::get_instance().scan(dllPaths);

			_cryptoengines.Clear();
			for (auto ng = srfg::cryptoselector::get_instance().engines_begin();
			         ng != srfg::cryptoselector::get_instance().engines_end(); ++ng)
			{
				_cryptoengines.Append(Sarcophagus::CryptoengineVM
				(
					ng->second->get_wname(),
					ng->first.raw64[0],
					ng->first.raw64[1]
				));
			}
		}
	}
}
