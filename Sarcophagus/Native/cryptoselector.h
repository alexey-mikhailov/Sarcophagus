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
		void scan(const std::vector<std::wstring>& file_paths);

		enum class switch_engine_result { success, module_not_found, does_not_implement_engine, could_not_instantiate };
		switch_engine_result switch_engine(std::string_view file_name);
		switch_engine_result switch_engine(const srfg::guid_t& uuid);

		std::shared_ptr<cryptoengine> get_engine() const { return _engine; }
		std::unordered_map<guid_t, std::shared_ptr<cryptoengine>>::const_iterator engines_begin() const { return _engines.begin(); }
		std::unordered_map<guid_t, std::shared_ptr<cryptoengine>>::const_iterator engines_end() const { return _engines.end(); }

	private:
		::HMODULE _module{};
		std::shared_ptr<cryptoengine> _engine;
		std::unordered_map<srfg::guid_t, std::shared_ptr<cryptoengine>> _engines;
	};
}
