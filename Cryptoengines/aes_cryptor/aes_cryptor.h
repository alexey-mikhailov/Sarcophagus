#pragma once
#include "pch.h"
#include <cryptoengine_abstracts.h>

#ifdef AES_CRYPTOR_EXPORTS
#define AES_CRYPTOR_API __declspec(dllexport)
#else
#define AES_CRYPTOR_API __declspec(dllimport)
#endif

extern "C"
{
	AES_CRYPTOR_API bool does_implement_srfg_cryptoengine();
	AES_CRYPTOR_API extern srfg::cryptoengine* instantiate_engine();
}

namespace srfg
{
	struct aes_cryptor final : cryptoengine
	{
		virtual ~aes_cryptor() override = default;
		virtual void setup(uint64_t size, const uint8_t* init_data) override;
		virtual void encrypt(uint64_t src_size, const uint8_t* src, uint64_t* dst_size, uint8_t** dst) override;
		virtual void decrypt(uint64_t src_size, const uint8_t* src, uint64_t* dst_size, uint8_t** dst) override;
		virtual guid_t get_uuid() const override;
		virtual std::string get_name() const override;
		virtual std::wstring get_wname() const override;

	private:
		std::vector<uint32_t> _expanded_key;

		/// Block size
		int _blksz;
		/// Width of key block
		int _blkwd;
		/// Number of round
		int _nr;

		uint8_t _state[4][4];

		uint8_t  mul(uint8_t a, uint8_t b);
		void     mix_columns();
		void     shift_rows();
		void     sub_bytes();
		uint8_t  sub_byte(uint8_t oneByte);
		uint32_t sub_word(uint32_t val);
		uint32_t rot_word(uint32_t val);
		void     inv_mix_columns();
		void     inv_shift_rows();
		uint8_t  inv_sub_byte(uint8_t oneByte);
		void     inv_sub_bytes();
		void     add_round_key(int roundNo);
		void     key_expansion(const uint8_t* key);
	};
}
