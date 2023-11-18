// cryptoengine_abstracts.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "cryptoengine_abstracts.h"

// TODO: For srfg::guid_t, is it safer to use SIMD?

namespace srfg
{
	bool guid_t::operator==(const guid_t& other) const
	{
		return raw64[0] == other.raw64[0] && raw64[1] == other.raw64[1];
	}

	bool guid_t::operator!=(const guid_t& other) const
	{
		return raw64[0] != other.raw64[0] || raw64[1] != other.raw64[1];
	}
}
