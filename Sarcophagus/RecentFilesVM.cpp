#include "pch.h"
#include "RecentFilesVM.h"

#include "ini_reflector.h"
#include "SarcophagusCommon.h"
#include "DialogTools.h"
#include "ViewModelHub.h"

#if __has_include("RecentFileVM.g.cpp")
#include "RecentFileVM.g.cpp"
#endif
#if __has_include("RecentFilesVM.g.cpp")
#include "RecentFilesVM.g.cpp"
#endif

namespace winrt::Sarcophagus::implementation
{
	winrt::Sarcophagus::RecentFilesVM RecentFileVM::RecentFilesVM() const
	{
		return ::Sarcophagus::ViewModelHub::GetInstance().RecentFilesVM();
	}

	void RecentFileVM::Name(const winrt::hstring& name)
	{
		if (name != _name)
		{
			_name = name;
			_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(wnameof(Name)));
		}
	}

	void RecentFileVM::File(const winrt::Windows::Storage::StorageFile& file)
	{
		if (file != _file)
		{
			_file = file;
			_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(wnameof(File)));
		}
	}

	winrt::event_token RecentFileVM::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value)
	{
		return _propertyChanged.add(value);
	}

	void RecentFileVM::PropertyChanged(const winrt::event_token& token)
	{
		_propertyChanged.remove(token);
	}

	RecentFilesVM::RecentFilesVM()
	{
		_recentFileNames = winrt::single_threaded_map<winrt::hstring, winrt::hstring>();
		_recentFiles = winrt::single_threaded_observable_vector<Sarcophagus::RecentFileVM>();
		InitAsync();
	}

	void RecentFilesVM::PushRecentFile(const StorageFile& file)
	{
		if (_recentFileNames.HasKey(file.Path()))
		{
			// Remove
			for (uint32_t i = 0; i < _recentFiles.Size(); ++i)
			{
				if (_recentFiles.GetAt(i).File().Path() == file.Path())
				{
					_recentFiles.RemoveAt(i);
				}
			}
		}
		else
		{
			_recentFileNames.Insert(file.Path(), file.Name());
		}

		_recentFiles.InsertAt(0, Sarcophagus::RecentFileVM(file.Name(), file));
		WriteIni();
	}

	void RecentFilesVM::WriteIni()
	{
		srfg::ini_reflector ini("Sarcophagus.ini");

		for (int i = 0; i < 4; ++i)
		{
			std::stringstream ss;
			ss << "RecentFile" << i;
			std::string parameterName = ss.str();

			if (_recentFiles.Size() > static_cast<uint32_t>(i))
			{
				const Sarcophagus::RecentFileVM& file = _recentFiles.GetAt(i);
				const StorageFile& storageFile = file.File();
				const hstring& path = storageFile.Path();
				std::string recentFilePath;
				std::transform(path.begin(), path.end(), std::back_inserter(recentFilePath), [](wchar_t c) { return static_cast<char>(c); });
				ini.set_parameter("RecentFiles", parameterName, recentFilePath);
			}
		}

		srfg::ini_reflector::write_result write_result = ini.write();
		SARCOPHAGUS_ASSERT(write_result == srfg::ini_reflector::write_result::success, 0, L"RecentFilesVM failed to write ini file. ");
	}

	IAsyncAction RecentFilesVM::InitAsync()
	{
		try
		{
			srfg::ini_reflector ini("Sarcophagus.ini");
			srfg::ini_reflector::read_result result = ini.read();
			if (result == srfg::ini_reflector::read_result::open_file_failure)
			{
				srfg::ini_reflector::write_result write_result = ini.write();
				SARCOPHAGUS_ASSERT(write_result == srfg::ini_reflector::write_result::success, 0, L"RecentFilesVM failed to write ini file. ");
				result = ini.read();
			}

			if (result == srfg::ini_reflector::read_result::success)
			{
				for (int i = 0; i < 4; ++i)
				{
					std::string recentFilePath;
					std::stringstream ss;
					ss << "RecentFile" << i;
					std::string parameterName = ss.str();
					ini.get_parameter("RecentFiles", parameterName, recentFilePath);

					if (!recentFilePath.empty())
					{
						std::wstring recentFileWide(recentFilePath.begin(), recentFilePath.end());
						StorageFile recentFile = co_await StorageFile::GetFileFromPathAsync(recentFileWide);
						_recentFileNames.Insert(recentFile.Path(), recentFile.Name());
						_recentFiles.Append(Sarcophagus::RecentFileVM(recentFile.Name(), recentFile));
					}
				}
			}
			else
			{
				SARCOPHAGUS_FAIL(0, L"RecentFilesVM failed to read ini file. ");
			}
		}
		catch(...)
		{
			SARCOPHAGUS_FAIL(0, L"RecentFilesVM failed to initialize. ");
		}
	}
}