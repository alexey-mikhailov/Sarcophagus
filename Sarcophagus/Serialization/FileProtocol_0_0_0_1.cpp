#include "pch.h"
#include "FileProtocol_0_0_0_1.h"

#include "cryptoselector.h"
#include "MainVM.h"
#include "SarcophagusCommon.h"
#include "ViewModelHub.h"

namespace Sarcophagus
{
	uint64_t FileProtocol_0_0_0_1::GetVersion()
	{
		return 0x00'00'00'01ui32;
	}

	IAsyncOperation<winrt::Sarcophagus::PullDataResult> FileProtocol_0_0_0_1::PullData(const winrt::Windows::Storage::Streams::DataReader& reader)
	{
		srfg::guid_t uuid;
		uuid.raw64[0] = reader.ReadUInt64();
		uuid.raw64[1] = reader.ReadUInt64();

		if (srfg::cryptoselector::get_instance().switch_engine(uuid) == srfg::cryptoselector::switch_engine_result::success)
		{
			if (const auto engine = srfg::cryptoselector::get_instance().get_engine())
			{
				winrt::Windows::Storage::Streams::IBuffer buffer = reader.DetachBuffer();

				uint64_t keySize = winrt::Sarcophagus::FileSerializer::GetInstance().GetStorageKeySize();
				uint8_t* keyBuff = reinterpret_cast<uint8_t*>(winrt::Sarcophagus::FileSerializer::GetInstance().GetStorageKeyBuff());
				engine->setup(keySize, keyBuff);

				uint64_t dstSize;
				uint8_t* dstData;
				engine->decrypt(buffer.Length(), buffer.data(), &dstSize, &dstData);
				uint8_t* dstPtr = dstData;

				srfg::guid_t uuidToCheck;
				uuidToCheck.raw64[0] = *reinterpret_cast<uint64_t*>(dstPtr);
				uuidToCheck.raw64[0] = _byteswap_uint64(uuidToCheck.raw64[0]);
				dstPtr += sizeof(uint64_t);
				uuidToCheck.raw64[1] = *reinterpret_cast<uint64_t*>(dstPtr);
				uuidToCheck.raw64[1] = _byteswap_uint64(uuidToCheck.raw64[1]);
				dstPtr += sizeof(uint64_t);

				if (uuid != uuidToCheck)
				{
					co_return winrt::Sarcophagus::PullDataResult::DecryptionFailure;
				}

				uint32_t keyInBytes = *reinterpret_cast<uint32_t*>(dstPtr);
				keyInBytes = _byteswap_ulong(keyInBytes);
				dstPtr += sizeof(uint32_t);

				SARCOPHAGUS_ASSERT(keyInBytes % 2 == 0, NULL, L"Number of bytes of cred.key should be even number. ");

				wchar_t* key = new wchar_t[keyInBytes / 2];
				memcpy(key, dstPtr, keyInBytes);
				dstPtr += keyInBytes;

				if (co_await winrt::Sarcophagus::FileSerializer::GetInstance().AuthAsync(keyInBytes, reinterpret_cast<uint64_t>(key)))
				{
					const winrt::Sarcophagus::MainVM vm = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
					vm.Credentials().Clear();

					uint32_t credentialCount = *reinterpret_cast<uint32_t*>(dstPtr);
					credentialCount = _byteswap_ulong(credentialCount);
					dstPtr += sizeof(uint32_t);

					for (uint32_t i = 0; i < credentialCount; ++i)
					{
						uint32_t nameInBytes = *reinterpret_cast<uint32_t*>(dstPtr);
						nameInBytes = _byteswap_ulong(nameInBytes);
						dstPtr += sizeof(uint32_t);

						SARCOPHAGUS_ASSERT(nameInBytes % 2 == 0, NULL, L"Number of bytes of cred.key should be even number. ");

						wchar_t* name = new wchar_t[nameInBytes / 2];
						memcpy(name, dstPtr, nameInBytes);
						dstPtr += nameInBytes;

						winrt::hstring credentialName(name, nameInBytes / 2);
						delete[] name;

						uint32_t passwordInBytes = *reinterpret_cast<uint32_t*>(dstPtr);
						passwordInBytes = _byteswap_ulong(passwordInBytes);
						dstPtr += sizeof(uint32_t);

						SARCOPHAGUS_ASSERT(passwordInBytes % 2 == 0, NULL, L"Number of bytes of cred.value should be even number. ");

						wchar_t* password = new wchar_t[passwordInBytes / 2];
						memcpy(password, dstPtr, passwordInBytes);
						dstPtr += passwordInBytes;

						winrt::hstring credentialPassword(password, passwordInBytes / 2);
						delete[] password;

						vm.Credentials().Append(winrt::Sarcophagus::Credential(credentialName, credentialPassword));
					}

					delete[] dstData;
				}

				delete[] key;
			}
		}

		co_return winrt::Sarcophagus::PullDataResult::Success;
	}

	IAsyncOperation<winrt::Sarcophagus::PushDataResult> FileProtocol_0_0_0_1::PushData(const winrt::Windows::Storage::Streams::DataWriter& writer)
	{
		if (const auto engine = srfg::cryptoselector::get_instance().get_engine())
		{
			const srfg::guid_t uuid = engine->get_uuid();
			writer.WriteUInt64(uuid.raw64[0]);
			writer.WriteUInt64(uuid.raw64[1]);

			const winrt::Windows::Storage::Streams::InMemoryRandomAccessStream rawStream;
			const winrt::Windows::Storage::Streams::DataWriter rawWriter(rawStream.GetOutputStreamAt(0));

			rawWriter.WriteUInt64(uuid.raw64[0]);
			rawWriter.WriteUInt64(uuid.raw64[1]);

			uint64_t keySize = winrt::Sarcophagus::FileSerializer::GetInstance().GetStorageKeySize();
			uint8_t* keyBuff = reinterpret_cast<uint8_t*>(winrt::Sarcophagus::FileSerializer::GetInstance().GetStorageKeyBuff());

			rawWriter.WriteUInt32(static_cast<uint32_t>(keySize));
			rawWriter.WriteBytes({ keyBuff, (uint32_t)keySize });
			engine->setup(keySize, keyBuff);

			const winrt::Sarcophagus::MainVM vm = ::Sarcophagus::ViewModelHub::GetInstance().MainVM();
			const uint32_t credentialCount = vm.Credentials().Size();
			rawWriter.WriteUInt32(credentialCount);

			for (auto credential : vm.Credentials())
			{
				const winrt::hstring name = credential.Name();
				const uint32_t nameInBytes = name.size() * 2;

				rawWriter.WriteUInt32(nameInBytes);
				rawWriter.WriteBytes({ (uint8_t*)name.c_str(), nameInBytes });

				const winrt::hstring password = credential.Password();
				const uint32_t passwordInBytes = password.size() * 2;

				rawWriter.WriteUInt32(passwordInBytes);
				rawWriter.WriteBytes({ (uint8_t*)password.c_str(), passwordInBytes });
			}

			const winrt::Windows::Storage::Streams::IBuffer srcBuffer = rawWriter.DetachBuffer();
			uint64_t dstSize;
			uint8_t* dstData;
			engine->encrypt(srcBuffer.Length(), srcBuffer.data(), &dstSize, &dstData);
			rawStream.Close();

			writer.WriteBytes(winrt::array_view<const uint8_t>(dstData, static_cast<uint32_t>(dstSize)));
			delete[] dstData;
		}

		co_return winrt::Sarcophagus::PushDataResult::Success;
	}
}
