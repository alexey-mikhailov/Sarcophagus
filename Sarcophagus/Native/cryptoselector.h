#pragma once
#include "cryptoengine_abstracts.h"

namespace srfg
{
	struct cryptoselector
	{
		static cryptoselector& get_instance()
		{
			static cryptoselector instance{};
			return instance;
		}

		void scan(const std::vector<std::string>& file_paths);

		enum class switch_engine_result { success, module_not_found, does_not_implement_engine, could_not_instantiate };
		switch_engine_result switch_engine(std::string_view file_name);
		switch_engine_result switch_engine(const srfg::guid_t& uuid);

		std::shared_ptr<cryptoengine> get_engine() const { return _engine; }

	private:
		::HMODULE _module{};
		std::shared_ptr<cryptoengine> _engine;
		std::unordered_map<srfg::guid_t, std::shared_ptr<cryptoengine>> _engines;
	};
}
