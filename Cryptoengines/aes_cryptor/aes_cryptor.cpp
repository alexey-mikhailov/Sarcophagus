#include "aes_cryptor.h"
#include "aes_constants.h"

bool does_implement_srfg_cryptoengine()
{
	return true;
}

srfg::cryptoengine* instantiate_engine()
{
	return new srfg::aes_cryptor();
}

namespace srfg
{
	void aes_cryptor::setup(uint64_t size, const uint8_t* init_data)
	{
		uint64_t new_key_size = size < 16 ? 16 : size < 24 ? 24 : 32;
		uint8_t* new_key = static_cast<uint8_t*>(malloc(new_key_size));
		memset(new_key, 0, new_key_size);

		uint64_t bytes_wrote = 0;
		uint64_t bytes_to_write;
		while (bytes_wrote < new_key_size)
		{
			bytes_to_write = (std::min)(new_key_size, size);
			memcpy(new_key + bytes_wrote, init_data + bytes_wrote % size, bytes_to_write);
			bytes_wrote += size;
		}

		_blksz = 4; // always 4                          // block size
		_blkwd = static_cast<int>(new_key_size / 4);     // width of key block
		_nr =    static_cast<int>(new_key_size / 4 + 6); // number of round

		_expanded_key.resize(_blkwd * (_nr + 1));

		key_expansion(new_key);
		free(new_key);
	}

	void aes_cryptor::encrypt(uint64_t src_size, const uint8_t* src, size_t* dst_size, uint8_t** dst)
	{
		*dst_size = 0;
		uint64_t cptwrd_size = 4 * _blksz;
		uint64_t output_offset = 0;
		uint8_t* output_buffer = nullptr;

		uint64_t appended_src_size = src_size + cptwrd_size - src_size % cptwrd_size;
		uint8_t* appended_input = static_cast<uint8_t*>(malloc(appended_src_size));
		memset(appended_input, 0, appended_src_size);
		memcpy(appended_input, src, src_size);

		while (output_offset < appended_src_size)
		{
			// copy one block into _state
			for (int r = 0; r < _blksz; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					_state[r][c] = appended_input[output_offset + (r * _blksz) + c];
				}
			}

			add_round_key(0);

			for (int i = 1; i < _nr; ++i)
			{
				sub_bytes();
				shift_rows();
				mix_columns();
				add_round_key(i);
			}

			sub_bytes();
			shift_rows();
			add_round_key(_nr);

			// TODO: Use allocator here.
			*dst_size += cptwrd_size;
			output_buffer = static_cast<uint8_t*>(realloc(output_buffer, *dst_size));
			memcpy(output_buffer + output_offset, _state, cptwrd_size);
			output_offset += cptwrd_size;
		}

		free(appended_input);
		*dst = output_buffer;
	}

	void aes_cryptor::decrypt(uint64_t src_size, const uint8_t* src, size_t* dst_size, uint8_t** dst)
	{
		*dst_size = 0;
		uint64_t cptwrd_size = 4 * _blksz;
		uint64_t output_offset = 0;
		uint8_t* output_buffer = nullptr;

		uint64_t appended_size = src_size + cptwrd_size - src_size % cptwrd_size;
		uint8_t* appended_input = static_cast<uint8_t*>(malloc(appended_size));
		memset(appended_input, 0, appended_size);
		memcpy(appended_input, src, src_size);

		while (output_offset < appended_size)
		{
			for (int r = 0; r < _blksz; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					_state[r][c] = appended_input[output_offset + (r * _blksz) + c];
				}
			}

			add_round_key(_nr);
			for (int i = _nr - 1; i > 0; --i)
			{
				inv_shift_rows();
				inv_sub_bytes();
				add_round_key(i);
				inv_mix_columns();
			}

			inv_sub_bytes();
			inv_shift_rows();
			add_round_key(0);

			// TODO: Use allocator here.
			*dst_size += cptwrd_size;
			output_buffer = static_cast<uint8_t*>(realloc(output_buffer, *dst_size));
			memcpy(output_buffer + output_offset, _state, cptwrd_size);
			output_offset += cptwrd_size;
		}

		free(appended_input);
		*dst = output_buffer;
	}

	guid_t aes_cryptor::get_uuid() const
	{
		return guid_t { 0x5058c6c9fa1783e8, 0x9de411d04e622a75 };
	}

	std::string aes_cryptor::get_name() const
	{
		return "AES";
	}

	std::wstring aes_cryptor::get_wname() const
	{
		return L"AES";
	}

	uint8_t aes_cryptor::mul(uint8_t a, uint8_t b)
	{
		if (a && b)
		{
			return alog_table
				[
					(static_cast<unsigned char>(log_table[a]) + static_cast<unsigned char>(log_table[b])) % 255
				];
		}

		return 0;
	}

	void aes_cryptor::inv_mix_columns()
	{
		uint8_t temp[4];

		for (int r = 0; r < _blksz; ++r)
		{
			// 4 rows and _blksz columns to store temp mix col value
			for (int c = 0; c < 4; ++c)
			{
				temp[c] = mul(inv_col_mix_matrix[c][0], (_state[r][0]))
					^ mul(inv_col_mix_matrix[c][1], (_state[r][1]))
					^ mul(inv_col_mix_matrix[c][2], (_state[r][2]))
					^ mul(inv_col_mix_matrix[c][3], (_state[r][3]));
			}

			_state[r][0] = temp[0];
			_state[r][1] = temp[1];
			_state[r][2] = temp[2];
			_state[r][3] = temp[3];
		}
	}

	void aes_cryptor::mix_columns()
	{
		uint8_t temp[4];

		for (int r = 0; r < _blksz; ++r)
		{
			// 4 rows and _blksz columns to store temp mix col value
			for (int c = 0; c < 4; ++c)
			{
				temp[c] = mul(col_mix_matrix[c][0], (_state[r][0]))
					^ mul(col_mix_matrix[c][1], (_state[r][1]))
					^ mul(col_mix_matrix[c][2], (_state[r][2]))
					^ mul(col_mix_matrix[c][3], (_state[r][3]));
			}

			_state[r][0] = temp[0];
			_state[r][1] = temp[1];
			_state[r][2] = temp[2];
			_state[r][3] = temp[3];
		}
	}

	void aes_cryptor::inv_shift_rows()
	{
		for (int c = 0; c < 4; ++c)
		{
			uint8_t temp[4];
			temp[0] = _state[0][c];
			temp[1] = _state[1][c];
			temp[2] = _state[2][c];
			temp[3] = _state[3][c];

			for (int r = 0; r < _blksz; ++r)
			{
				_state[(c + r) % _blksz][c] = temp[r];
			}
		}
	}

	void aes_cryptor::shift_rows()
	{
		for (int c = 0; c < 4; ++c)
		{
			uint8_t temp[4];
			for (int r = 0; r < _blksz; ++r)
			{
				temp[r] = _state[(c + r) % _blksz][c];
			}

			_state[0][c] = temp[0];
			_state[1][c] = temp[1];
			_state[2][c] = temp[2];
			_state[3][c] = temp[3];
		}
	}

	uint8_t aes_cryptor::inv_sub_byte(uint8_t oneByte)
	{
		// 4 bits for row index and 4 bits for column index
		return si_box[oneByte];
	}

	void aes_cryptor::inv_sub_bytes()
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int i = 0; i < _blksz; ++i)
			{
				_state[i][j] = inv_sub_byte(_state[i][j]);
			}
		}
	}

	void aes_cryptor::sub_bytes()
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int i = 0; i < _blksz; ++i)
			{
				_state[i][j] = sub_byte(_state[i][j]);
			}
		}
	}

	uint8_t aes_cryptor::sub_byte(uint8_t oneByte)
	{
		// one byte represent in hex (xy)
		// x is row index and y is column index
		return s_box[oneByte];
	}

	uint32_t aes_cryptor::sub_word(uint32_t val)
	{
		uint8_t  oneByte;
		uint32_t res = 0;

		for (int i = 0; i < 4; ++i)
		{
			res = res << 8;
			oneByte = (val >> 24) & 0xFF;
			res = res | sub_byte(oneByte);
			val = val << 8;
		}

		return res;
	}

	uint32_t aes_cryptor::rot_word(uint32_t val)
	{
		uint32_t res = val << 8;
		res = res | (val >> 24);
		return res;
	}

	void aes_cryptor::add_round_key(int roundNo)
	{
		for (int col = 0; col < _blksz; ++col)
		{
			uint32_t roundKeyVal = _expanded_key[(roundNo * _blksz) + col];

			for (int row = 3; row >= 0; --row)
			{
				_state[col][row] ^= (roundKeyVal & 0xFF);
				roundKeyVal = roundKeyVal >> 8;
			}
		}
	}

	void aes_cryptor::key_expansion(const uint8_t* key)
	{
		uint32_t temp;
		for (int i = 0; i < _blkwd; ++i)
		{
			_expanded_key[i] = ((unsigned char)key[4 * i + 0] << 24) |
				((unsigned char)key[4 * i + 1] << 16) |
				((unsigned char)key[4 * i + 2] << 8) |
				((unsigned char)key[4 * i + 3] << 0);
		}

		for (int i = _blkwd; i < _blksz * (_nr + 1); ++i)
		{
			temp = _expanded_key[i - 1];

			if (i % _blkwd == 0)
			{
				temp = sub_word(rot_word(temp)) ^ (r_con[i / _blkwd] << 24);
			}
			else if (_blkwd > 6 && i % _blkwd == 4)
			{
				temp = sub_word(temp);
			}

			_expanded_key[i] = _expanded_key[i - _blkwd] ^ temp;
		}
	}
}
