#pragma once
#include "FileSerializer.g.h"

namespace Sarcophagus
{
	struct FileProtocol;
}

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Data;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;

namespace winrt::Sarcophagus::implementation
{
    struct FileSerializer : FileSerializerT<FileSerializer>
    {
        static Sarcophagus::FileSerializer GetInstance()
        {
            static Sarcophagus::FileSerializer instance = winrt::make<Sarcophagus::implementation::FileSerializer>();
            return instance;
        }

        FileSerializer();
        virtual ~FileSerializer() override;
        uint64_t GetStorageKeySize() const { return _storageKeySize; }
        uint64_t GetStorageKeyBuff() const { return reinterpret_cast<uint64_t>(_storageKeyBuff); }
        void SetStorageKey(uint64_t size, uint64_t buff);

        bool IsDirty() const { return _isDirty; }
        Visibility IsDirtyVisibility() const { return _isDirty ? Visibility::Visible : Visibility::Collapsed; }
        void IsDirtyVisibility(Visibility value) { _isDirty = value == Visibility::Visible; }
        void MakeDirty();
        void ClearDirty();

        void Initialize();
        void OpenFile(const StorageFile& file);
        void SaveFile(const StorageFile& file);

        IAsyncAction OpenFileAsync(const StorageFile& file);
        IAsyncAction SaveFileAsync(const StorageFile& file);

        IAsyncOperation<bool> AuthAsync(uint64_t keySize, uint64_t keyBuff);

        winrt::event_token PropertyChanged(PropertyChangedEventHandler const& value);
        void PropertyChanged(const winrt::event_token& token);

    private:
        bool _isDirty;
        uint32_t _currentVersion;
        uint64_t _storageKeySize;
        uint8_t* _storageKeyBuff;
        std::unordered_map <uint32_t, std::shared_ptr<::Sarcophagus::FileProtocol>> _protocols;
        winrt::event<PropertyChangedEventHandler> _propertyChanged;
    };
}

namespace winrt::Sarcophagus::factory_implementation
{
    struct FileSerializer : FileSerializerT<FileSerializer, implementation::FileSerializer> {};
}
