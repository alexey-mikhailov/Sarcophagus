#pragma once
#include "RecentFileVM.g.h"
#include "RecentFilesVM.g.h"

using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Sarcophagus::implementation
{
	struct RecentFileVM : RecentFileVMT<RecentFileVM>
	{
		RecentFileVM(winrt::hstring name, const StorageFile& file)
			: _name(name)
			, _file(file)
		{}

		winrt::Sarcophagus::RecentFilesVM RecentFilesVM() const;

		winrt::hstring Name() const { return _name; }
		void Name(const winrt::hstring& name);
		StorageFile File() const { return _file; }
		void File(const StorageFile& storageFile);

		winrt::event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value);
		void PropertyChanged(const winrt::event_token& token);

	private:
		winrt::hstring _name;
		StorageFile _file;
		winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
	};

	struct RecentFilesVM : RecentFilesVMT<RecentFilesVM>
	{
		RecentFilesVM();
		void PushRecentFile(const StorageFile& file);
		void WriteIni();

		IObservableVector<Sarcophagus::RecentFileVM> RecentFiles() const { return _recentFiles; }
		Sarcophagus::ChooseRecentFileCommand ChooseRecentFileCommand() const { return _chooseRecentFileCommand; }

	private:
		IAsyncAction InitAsync();
		IMap<winrt::hstring, winrt::hstring> _recentFileNames;
		IObservableVector<Sarcophagus::RecentFileVM> _recentFiles;
		Sarcophagus::ChooseRecentFileCommand _chooseRecentFileCommand;
	};
}

namespace winrt::Sarcophagus::factory_implementation
{
	struct RecentFileVM : RecentFileVMT<RecentFileVM, implementation::RecentFileVM> {};
	struct RecentFilesVM : RecentFilesVMT<RecentFilesVM, implementation::RecentFilesVM> {};
}
