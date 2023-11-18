#pragma once
#include "FileProtocol.h"

namespace Sarcophagus
{
	struct FileProtocol_0_0_0_1 : FileProtocol
	{
		uint64_t GetVersion() override;
		IAsyncOperation<winrt::Sarcophagus::PullDataResult> PullData(const winrt::Windows::Storage::Streams::DataReader& reader) override;
		IAsyncOperation<winrt::Sarcophagus::PushDataResult> PushData(const winrt::Windows::Storage::Streams::DataWriter& writer) override;
	};
}
