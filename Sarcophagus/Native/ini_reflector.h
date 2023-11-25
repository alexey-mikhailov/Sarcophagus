#pragma once

namespace srfg
{
	struct section
	{
		std::string name;
		std::map<std::string, std::string> values;
	};

	struct ini_reflector
	{
		explicit ini_reflector(const std::string& filename);

		std::tuple<std::map<std::string, section>::iterator, std::map<std::string, section>::iterator> get_sections() { return { _sections.begin(), _sections.end() }; }
		section& get_section_ref(const std::string& name) { return _sections[name]; }

		enum class get_parameter_result { success, section_not_found, parameter_not_found };
		get_parameter_result get_parameter(const std::string& section_name, const std::string& parameter_name, std::string& parameter_value);
		void set_parameter(const std::string& section_name, const std::string& parameter_name, const std::string& parameter_value);

		enum class read_result { success, open_file_failure, incomplete_section_name, expected_equal_sign };
		read_result read();

		enum class write_result { success, open_file_failure };
		write_result write();

	private:
		std::string _filename;
		std::map<std::string, section> _sections;
	};


}