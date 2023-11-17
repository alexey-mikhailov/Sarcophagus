#include "pch.h"
#include "InternalCryptoTool.h"

namespace Sarcophagus
{
	InternalCryptoTool::~InternalCryptoTool()
	{
		delete[] _key;
	}

	void InternalCryptoTool::Setup(uint64_t size, const uint8_t* data)
	{
		_keySize = size;

		// Will be removed at dtor. 
		_key = new uint8_t[size];
		memcpy(_key, data, size);
	}

	InternalCryptoTool::EncryptResult
	InternalCryptoTool::Encrypt(uint64_t srcSize, const uint8_t* src, uint64_t* dstSize, uint8_t** dst)
	{
		*dstSize = srcSize;
		*dst = new uint8_t[*dstSize];

		if (_keySize > 0)
		{
			const uint8_t* srcPtr = src - 1;
			uint8_t* dstPtr = *dst - 1;
			uint64_t index = 0;
			uint64_t keyIndex = 0;

			for (uint64_t i = 0; i < srcSize; ++i)
			{
				uint8_t mod = 0;
				for (uint64_t j = keyIndex; j < keyIndex + _keySize * 1.45f; ++j)
				{
					mod ^= 0x100 - _key[j % _keySize] + i + ++index;
				}

				*++dstPtr = *++srcPtr ^ mod;

				if (++keyIndex > _keySize)
				{
					keyIndex = 0;
				}
			}
		}
		else
		{
			return EncryptResult::NeededSetup;
		}

		return EncryptResult::Success;
	}

	InternalCryptoTool::DecryptResult
	InternalCryptoTool::Decrypt(uint64_t srcSize, const uint8_t* src, uint64_t* dstSize, uint8_t** dst)
	{
		*dstSize = srcSize;
		*dst = new uint8_t[*dstSize];

		if (_keySize > 0)
		{
			const uint8_t* srcPtr = src - 1;
			uint8_t* dstPtr = *dst - 1;
			uint64_t index = 0;
			uint64_t keyIndex = 0;

			for (uint64_t i = 0; i < srcSize; ++i)
			{
				uint8_t mod = 0;
				for (uint64_t j = keyIndex; j < keyIndex + _keySize * 1.45f; ++j)
				{
					mod ^= 0x100 - _key[j % _keySize] + i + ++index;
				}

				*++dstPtr = *++srcPtr ^ mod;

				if (++keyIndex > _keySize)
				{
					keyIndex = 0;
				}
			}
		}
		else
		{
			return DecryptResult::NeededSetup;
		}

		return DecryptResult::Success;
	}
}