#pragma once
#include "pch.h"

namespace winrt
{
	namespace Sarcophagus
	{
		enum class PushDataResult;
		enum class PullDataResult;
	}
}

namespace Sarcophagus
{
    using namespace winrt::Windows::Foundation;

    struct FileProtocol
    {
        virtual ~FileProtocol() = default;
        virtual uint64_t GetVersion() = 0;

    	virtual IAsyncOperation<winrt::Sarcophagus::PullDataResult> PullData(const winrt::Windows::Storage::Streams::DataReader& reader) = 0;
        virtual IAsyncOperation<winrt::Sarcophagus::PushDataResult> PushData(const winrt::Windows::Storage::Streams::DataWriter& writer) = 0;
    };
}
