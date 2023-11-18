#include "enginema.h"

bool does_implement_srfg_cryptoengine()
{
	return true;
}

srfg::cryptoengine* instantiate_engine()
{
	return new srfg::enginema();
}

namespace srfg
{
	void enginema::setup(uint64_t size, const uint8_t* init_data)
	{
		_count = 0;

		_first_rotor = create_rotor(size, init_data);                      // Creating the first rotor
		_rotor_copies.push_back(_first_rotor);                             // Assign the first rotor

		std::string second_key_str = "again";
		std::vector<uint8_t> second_key(second_key_str.begin(), second_key_str.end());

		_second_rotor = create_rotor(second_key.size(), second_key.data()); // Creating the second rotor
		_rotor_copies.push_back(_second_rotor);                             // Assign the second rotor

		std::string third_key_str = "fortune";
		std::vector<uint8_t> third_key(third_key_str.begin(), third_key_str.end());

		_third_rotor = create_rotor(third_key.size(), third_key.data()); // Creating the third rotor
		_rotor_copies.push_back(_third_rotor);                           // Assign the third rotor
	}

	void enginema::encrypt(uint64_t src_size, const uint8_t* src, size_t* dst_size, uint8_t** dst)
	{
		*dst_size = src_size;
		uint8_t* dstv = *dst = new uint8_t[*dst_size];

		--src; --dstv;
		for (uint64_t i = 0; i < src_size; ++i)
		{
			*++dstv = encode_byte(*++src);
		}
	}

	void enginema::decrypt(uint64_t src_size, const uint8_t* src, size_t* dst_size, uint8_t** dst)
	{
		*dst_size = src_size;
		uint8_t* dstv = *dst = new uint8_t[*dst_size];

		--src; --dstv;
		for (uint64_t i = 0ui64; i < src_size; ++i)
		{
			*++dstv = decode_byte(*++src);
		}
	}

	guid_t enginema::get_uuid() const
	{
		return guid_t { 0x34ae9cbf5d9c10ff, 0x04be57a1b3c9d0b0 };
	}

	std::vector<std::pair<int, int>> enginema::create_rotor(size_t key_size, uint8_t const* key)
	{
		std::vector<std::pair<int, int>> rotor;
		size_t key_left = 0x100;

		while (key_left)
		{
			uint8_t v = key[(0x100 - key_left) % key_size];

			bool searching = true;
			int step = 10 * key_size;
			while (searching)
			{
				searching = false;

				for (auto pair : rotor)
				{
					if (pair.second == v)
					{
						v = (v + step) % 0x100;
						searching = true;
						if (step > 1) --step;
						break;
					}
				}
			}

			rotor.emplace_back(0x100 - key_left, v);
			--key_left;
		}

		return rotor;
	}

	uint8_t enginema::encode_byte(uint8_t ch)
	{
		++_count;
		std::pair<int, int> front;

		// Finding the specific position for each of the character
		int index = 0;
		for (const auto& pair : _first_rotor)
		{
			if (pair.second == ch)
			{
				break;
			}

			++index;
		}

		// Rotating the first rotor
		front = _first_rotor.front();
		_first_rotor.erase(_first_rotor.begin());
		_first_rotor.push_back(front);

		ch = _second_rotor[index].first;

		// Finding the specific position for each of the character
		index = 0;
		for (const auto& pair : _second_rotor)
		{
			if (pair.second == ch)
			{
				break;
			}

			++index;
		}

		// Rotating the second rotor
		if (_count % 0x100 == 0)
		{
			front = _second_rotor.front();
			_second_rotor.erase(_second_rotor.begin());
			_second_rotor.push_back(front);
		}

		ch = _third_rotor[index].first;

		// Rotating the third rotor
		if (_count % 0x10000 == 0)
		{
			front = _third_rotor.front();
			_third_rotor.erase(_third_rotor.begin());
			_third_rotor.push_back(front);
		}

		// Converting Decimal to ASCII
		return ch;
	}

	uint8_t enginema::decode_byte(uint8_t ch)
	{
		++_count;
		std::pair<int, int> front;

		// Finding the specific position for each of the character
		int index = 0;
		for (const auto& pair : _third_rotor)
		{
			if (pair.first == ch)
			{
				break;
			}

			++index;
		}

		// Rotating the third rotor
		if (_count % 0x10000 == 0)
		{
			front = _third_rotor.front();
			_third_rotor.erase(_third_rotor.begin());
			_third_rotor.push_back(front);
		}

		ch = _second_rotor[index].second;

		// Finding the specific position for each of the character
		index = 0;
		for (const auto& pair : _second_rotor)
		{
			if (pair.first == ch)
			{
				break;
			}

			++index;
		}

		// Rotating the second rotor
		if (_count % 0x100 == 0)
		{
			front = _second_rotor.front();
			_second_rotor.erase(_second_rotor.begin());
			_second_rotor.push_back(front);
		}

		ch = _first_rotor[index].second;

		// Rotating the first rotor
		front = _first_rotor.front();
		_first_rotor.erase(_first_rotor.begin());
		_first_rotor.push_back(front);

		// Converting Decimal to ASCII
		return ch;
	}
}
