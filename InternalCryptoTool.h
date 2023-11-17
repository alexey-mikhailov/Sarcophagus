#pragma once

namespace Sarcophagus
{
	struct InternalCryptoTool
	{
		static InternalCryptoTool& GetInstance()
		{
			static InternalCryptoTool instance;
			return instance;
		}

		virtual ~InternalCryptoTool();
		void Setup(uint64_t size, const uint8_t* data);

		enum class EncryptResult { Success, NeededSetup };
		EncryptResult Encrypt(uint64_t srcSize, const uint8_t* src, uint64_t* dstSize, uint8_t** dst);
		enum class DecryptResult { Success, NeededSetup };
		DecryptResult Decrypt(uint64_t srcSize, const uint8_t* src, uint64_t* dstSize, uint8_t** dst);

	private:
		uint64_t _keySize = 0;
		uint8_t* _key = nullptr;
	};
}