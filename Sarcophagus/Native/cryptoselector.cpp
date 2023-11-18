#include "pch.h"
#include "cryptoselector.h"

namespace srfg
{
	void cryptoselector::scan(const std::vector<std::string>& file_paths)
	{
		_engines.clear();

		// Scan files
		for (auto file_path : file_paths)
		{
			::HMODULE module{};

			if (file_path[file_path.size() - 1])
			{
				// Non-zero terminated source. 
				char* sz_name = new char[file_path.size() + 1];
				memcpy_s(sz_name, file_path.size(), file_path.data(), file_path.size());
				sz_name[file_path.size()] = '\0';
				module = ::LoadLibraryA(sz_name);
				delete[] sz_name;
			}
			else
			{
				// Zero-terminated source. 
				module = ::LoadLibraryA(file_path.data());
			}

			if (module)
			{
				typedef bool(*does_implement_srfg_cryptoengine_t)(void);
				auto does_implement_srfg_cryptoengine = (::GetProcAddress(module, "does_implement_srfg_cryptoengine"));

				if (does_implement_srfg_cryptoengine && does_implement_srfg_cryptoengine())
				{
					typedef srfg::cryptoengine* (*instantiate_engine_t)(void);
					const auto instantiate_engine = reinterpret_cast<instantiate_engine_t>(::GetProcAddress(module, "instantiate_engine"));

					if (instantiate_engine)
					{
						if (auto engine = instantiate_engine())
						{
							_engines[engine->get_uuid()] = std::shared_ptr<srfg::cryptoengine>(engine);
						}
					}
				}
			}
		}
	}

	cryptoselector::switch_engine_result cryptoselector::switch_engine(std::string_view file_name)
	{
		::HMODULE module{};

		if (file_name[file_name.size() - 1])
		{
			// Non-zero terminated source. 
			char* sz_name = new char[file_name.size() + 1];
			memcpy_s(sz_name, file_name.size(), file_name.data(), file_name.size());
			sz_name[file_name.size()] = '\0';
			module = ::LoadLibraryA(sz_name);
			delete[] sz_name;
		}
		else
		{
			// Zero-terminated source. 
			module = ::LoadLibraryA(file_name.data());
		}

		if (module)
		{
			typedef bool(* does_implement_srfg_cryptoengine_t)(void);
			auto does_implement_srfg_cryptoengine = (::GetProcAddress(module, "does_implement_srfg_cryptoengine"));

			if (does_implement_srfg_cryptoengine && does_implement_srfg_cryptoengine())
			{
				typedef srfg::cryptoengine* (* instantiate_engine_t)(void);
				const auto instantiate_engine = reinterpret_cast<instantiate_engine_t>(::GetProcAddress(module, "instantiate_engine"));

				if (instantiate_engine)
				{
					if (auto engine = instantiate_engine())
					{
						_engine = std::shared_ptr<srfg::cryptoengine>(engine);
						if (_engine)
						{
							_engines[_engine->get_uuid()] = _engine;
						}

						_module = module;
						return switch_engine_result::success;
					}
				}

				return switch_engine_result::could_not_instantiate;
			}

			return switch_engine_result::does_not_implement_engine;
		}

		return switch_engine_result::module_not_found;
	}

	cryptoselector::switch_engine_result cryptoselector::switch_engine(const srfg::guid_t& uuid)
	{
		for (const auto& engine : _engines)
		{
			if (engine.second && engine.second->get_uuid() == uuid)
			{
				_engine = engine.second;
				return switch_engine_result::success;
			}
		}

		return switch_engine_result::module_not_found;
	}
}
