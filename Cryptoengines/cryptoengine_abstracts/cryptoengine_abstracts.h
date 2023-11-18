#pragma once

namespace srfg
{
	/// 128-bit unique identifier 
	union guid_t
	{
		uint64_t raw64[2];
		uint32_t raw32[4];
		uint16_t raw16[8];
		uint8_t  raw08[16];

		bool operator==(const guid_t& other) const;
		bool operator!=(const guid_t& other) const;
	};
}

namespace std
{
	template <>
	struct hash<srfg::guid_t>
	{
		size_t operator()(const srfg::guid_t& value) const noexcept
		{
			return hash<uint64_t>()(value.raw64[0]) ^ hash<uint64_t>()(value.raw64[1]);
		}
	};
}

namespace srfg
{
	struct cryptoengine
	{
		virtual ~cryptoengine() = default;
		virtual void setup(uint64_t size, const uint8_t* init_data) = 0;
		virtual void encrypt(uint64_t src_size, const uint8_t* src, uint64_t* dst_size, uint8_t** dst) = 0;
		virtual void decrypt(uint64_t src_size, const uint8_t* src, uint64_t* dst_size, uint8_t** dst) = 0;
		virtual guid_t get_uuid() const = 0;
		virtual std::string get_name() const = 0;
		virtual std::wstring get_wname() const = 0;
	};
}
