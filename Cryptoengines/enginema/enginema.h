#pragma once
#include "pch.h"
#include <cryptoengine_abstracts.h>

#ifdef ENGINEMA_EXPORTS
#define ENGINEMA_API __declspec(dllexport)
#else
#define ENGINEMA_API __declspec(dllimport)
#endif

extern "C"
{
	ENGINEMA_API bool does_implement_srfg_cryptoengine();
	ENGINEMA_API extern srfg::cryptoengine* instantiate_engine();
}

namespace srfg
{
	struct enginema final : cryptoengine
	{
		virtual ~enginema() override = default;
		virtual void setup(uint64_t size, const uint8_t* init_data) override;
		virtual void encrypt(uint64_t src_size, const uint8_t* src, uint64_t* dst_size, uint8_t** dst);
		virtual void decrypt(uint64_t src_size, const uint8_t* src, uint64_t* dst_size, uint8_t** dst);
		virtual guid_t get_uuid() const override;
		virtual std::string get_name() const override;
		virtual std::wstring get_wname() const override;

		std::vector<std::pair<int, int>> get_first_rotor() const { return _first_rotor; }
		std::vector<std::pair<int, int>> get_second_rotor() const { return _second_rotor; }
		std::vector<std::pair<int, int>> get_third_rotor() const { return _third_rotor; }

	private:
		std::vector<std::pair<int, int>> create_rotor(size_t key_size, uint8_t const* key);

		uint8_t encode_byte(uint8_t ch);
		uint8_t decode_byte(uint8_t ch);

		std::vector<std::pair<int, int>> _first_rotor;
		std::vector<std::pair<int, int>> _second_rotor;
		std::vector<std::pair<int, int>> _third_rotor;
		std::vector<std::vector<std::pair<int, int>>> _rotor_copies;
		int _count = 0;
	};
}
