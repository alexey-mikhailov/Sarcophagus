#include "pch.h"
#include "FileSerializer.h"

#include "App.xaml.h"
#include "DialogTools.h"
#include "FileProtocol_0_0_0_1.h"
#include "InternalCryptoTool.h"
#include "SarcophagusCommon.h"

#if __has_include("FileSerializer.g.cpp")
#include "FileSerializer.g.cpp"
#endif

namespace winrt::Sarcophagus::implementation
{
	FileSerializer::FileSerializer()
		: _currentVersion(0x00'00'00'01ui32)
	{
		// Change current version in member initializer. 
	}

	FileSerializer::~FileSerializer()
	{
		delete[] _storageKeyBuff;
	}

	void FileSerializer::SetStorageKey(uint64_t size, uint64_t buff)
	{
		if (_storageKeyBuff)
		{
			delete[] _storageKeyBuff;
		}

		_storageKeySize = size;
		_storageKeyBuff = new uint8_t[size];
		memcpy(_storageKeyBuff, reinterpret_cast<void*>(buff), size);
	}

	void FileSerializer::MakeDirty()
	{
		if (!_isDirty)
		{
			_isDirty = true;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(IsDirty)));
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(IsDirtyVisibility)));
		}
	}

	void FileSerializer::ClearDirty()
	{
		if (_isDirty)
		{
			_isDirty = false;
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(IsDirty)));
			_propertyChanged(*this, PropertyChangedEventArgs(wnameof(IsDirtyVisibility)));
		}
	}

	void FileSerializer::Initialize()
	{
		// Add new file versions here. 
		_protocols.emplace(0x00'00'00'01ui32, std::make_shared<::Sarcophagus::FileProtocol_0_0_0_1>());
	}

	void FileSerializer::OpenFile(const StorageFile& file)
	{
		OpenFileAsync(file);
	}

	void FileSerializer::SaveFile(const StorageFile& file)
	{
		SaveFileAsync(file);
	}

	IAsyncAction FileSerializer::OpenFileAsync(const StorageFile& file)
	{
		if (Streams::IRandomAccessStream srcStream = co_await file.OpenAsync(FileAccessMode::Read))
		{
			const Streams::Buffer srcBuffer(static_cast<uint32_t>(srcStream.Size()));
			co_await srcStream.ReadAsync(srcBuffer, static_cast<uint32_t>(srcStream.Size()), Streams::InputStreamOptions::None);

			winrt::Windows::Storage::Streams::DataReader reader(srcStream.GetInputStreamAt(0));
			if (co_await reader.LoadAsync(sizeof(uint32_t)))
			{
				const uint32_t fileVersion = reader.ReadUInt32();
				reader = winrt::Windows::Storage::Streams::DataReader(srcStream.GetInputStreamAt(sizeof(uint32_t)));

				if (co_await reader.LoadAsync(static_cast<uint32_t>(srcStream.Size() - sizeof(uint32_t))))
				{
					const auto it = _protocols.find(fileVersion);
					if (it != _protocols.end())
					{
						if (auto fileProtocol = it->second)
						{
							const auto pullDataResult = co_await fileProtocol->PullData(reader);
							winrt::hstring errorText;
							switch (pullDataResult)
							{
							case PullDataResult::Success: 
								errorText = L"Pull data finished successful. ";
								break;
							case PullDataResult::DecryptionFailure: 
								errorText = L"Pull data failed due to decryption failure. ";
								break;
							}
							
							if (pullDataResult != PullDataResult::Success)
							{
								co_await ::Sarcophagus::ShowErrorAsync(errorText);
							}
						}
					}
				}
			}

			reader.Close();
			srcStream.Close();
		}
	}

	IAsyncAction FileSerializer::SaveFileAsync(const StorageFile& file)
	{
		const winrt::Windows::Storage::Streams::InMemoryRandomAccessStream memStream;
		const winrt::Windows::Storage::Streams::DataWriter writer(memStream.GetOutputStreamAt(0));

		writer.WriteUInt32(_currentVersion);

		const auto it = _protocols.find(_currentVersion);
		if (it != _protocols.end())
		{
			if (auto fileProtocol = it->second)
			{
				co_await fileProtocol->PushData(writer);
			}
		}

		if (Streams::IRandomAccessStream dskStream = co_await file.OpenAsync(FileAccessMode::ReadWrite))
		{
			const Streams::IBuffer buffer = writer.DetachBuffer();
			dskStream.Size(buffer.Length());

			co_await dskStream.WriteAsync(buffer);
			if (co_await dskStream.FlushAsync())
			{
				dskStream.Close();
			}
		}
	}

	IAsyncOperation<bool> FileSerializer::AuthAsync(uint64_t keySize, uint64_t keyBuff)
	{
		bool pwdOk = keySize == _storageKeySize;
		if (pwdOk)
		{
			pwdOk &= memcmp(reinterpret_cast<void*>(keyBuff), _storageKeyBuff, _storageKeySize) == 0;
		}

		if (pwdOk)
		{
			co_return true;
		}
		else
		{
			co_await ::Sarcophagus::ShowErrorAsync(L"Wrong password. ");
			co_return false;
		}
	}

	event_token FileSerializer::PropertyChanged(PropertyChangedEventHandler const& value)
	{
		return _propertyChanged.add(value);
	}

	void FileSerializer::PropertyChanged(const winrt::event_token& token)
	{
		_propertyChanged.remove(token);
	}
}