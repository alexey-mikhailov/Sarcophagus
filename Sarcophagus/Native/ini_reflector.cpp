#include "pch.h"
#include "ini_reflector.h"

namespace srfg
{
	char* skip_one_line_break(char* begin, char* end)
	{
		char* crpos = strchr(begin, '\r');
		char* lfpos = strchr(begin, '\n');
		if (crpos && crpos < end && lfpos && lfpos < end)
		{
			if (lfpos == crpos + 1)
			{
				// Windows case
				return lfpos + 1;
			}
		}
		else if (lfpos && lfpos < end)
		{
			// Linux, MacOS
			return lfpos + 1;
		}
		else if (crpos && crpos < end)
		{
			// Vintage case (Commodore...)
			return crpos + 1;
		}

		return nullptr;
	}

	void truncate(char* begin, char* end, char** new_begin, char** new_end)
	{
		*new_begin = begin;
		*new_end = end;

		while (begin < end)
		{
			if (*begin != ' ' && *begin != '\r' && *begin != '\n' && *begin != '\t')
			{
				*new_begin = begin;
				break;
			}
			++begin; ++* new_begin;
		}

		while (end > begin)
		{
			if (*end != ' ' && *end != '\r' && *end != '\n' && *end != '\t')
			{
				*new_end = end + 1;
				break;
			}
			--end; --* new_end;
		}
	}

	ini_reflector::ini_reflector(const std::string& filename) : _filename(filename)
	{
	}

	ini_reflector::get_parameter_result ini_reflector::get_parameter(const std::string& section_name,
		const std::string& parameter_name,
		std::string& parameter_value)
	{
		const auto section = _sections.find(section_name);
		if (section != _sections.end())
		{
			for (const auto& parameter : section->second.values)
			{
				if (parameter.first == parameter_name)
				{
					parameter_value = parameter.second;
					return get_parameter_result::success;
				}
			}

			return get_parameter_result::parameter_not_found;
		}

		return get_parameter_result::section_not_found;
	}

	void ini_reflector::set_parameter(const std::string& section_name,
		const std::string& parameter_name,
		const std::string& parameter_value)
	{
		auto& section = _sections[static_cast<std::string>(section_name)];
		section.values.insert_or_assign(parameter_name,
			parameter_value);
	}

	ini_reflector::read_result ini_reflector::read()
	{
		// Read file to buffer
		std::ifstream is(_filename, std::ios_base::binary);
		if (!is.is_open())
		{
			return read_result::open_file_failure;
		}

		is.seekg(0, is.end);
		const uint64_t size = is.tellg();
		is.seekg(0);

		char* fileContent = new char[size + 1];
		is.read(fileContent, static_cast<int64_t>(size));
		fileContent[size] = '\0';

		// Parse file from buffer
		_sections.clear();
		std::vector<std::string> lines;
		char* pSection0 = strchr(fileContent, '[');
		while (pSection0 && pSection0 < fileContent + size)
		{
			if (char* pSection1 = strchr(pSection0, ']'))
			{
				std::string sectionName(pSection0 + 1, pSection1);
				pSection0 = strchr(pSection1, '[');
				if (pSection0 == nullptr)
				{
					pSection0 = fileContent + size;
				}

				section section{ sectionName, {} };

				// Read section parameters
				char* pParameter0 = skip_one_line_break(pSection1 + 1, pSection0);
				char* pParameter1 = skip_one_line_break(pParameter0, pSection0);
				while (pParameter1 && pParameter1 < pSection0)
				{
					// Read section parameter
					if (*pParameter0 == ';')
					{
						// Commented line. Skip it. 
					}
					else
					{
						if (char* pEqualSign = strchr(pParameter0, '='))
						{
							char* pParameterName0, * pParameterName1, * pParameterValue0, * pParameterValue1;
							truncate(pParameter0, pEqualSign - 1, &pParameterName0, &pParameterName1);
							truncate(pEqualSign + 1, pParameter1 - 1, &pParameterValue0, &pParameterValue1);
							section.values[std::string(pParameterName0, pParameterName1)] = std::string(pParameterValue0, pParameterValue1);
						}
						else
						{
							char* p0 = pParameter0;
							char* p1 = skip_one_line_break(p0, pSection0);
							truncate(p0, p1, &p0, &p1);

							if (p0 == p1)
							{
								// Just empty line. Let's go on. 
							}
							else
							{
								return read_result::expected_equal_sign;
							}
						}
					}

					pParameter0 = pParameter1;
					pParameter1 = skip_one_line_break(pParameter0, pSection0);
				}

				_sections.emplace(section.name, section);
			}
			else
			{
				return read_result::incomplete_section_name;
			}
		}

		delete[] fileContent;
		return read_result::success;
	}

	ini_reflector::write_result ini_reflector::write()
	{
		// Stringize file content.
		std::stringstream ss;

		for (const auto& section : _sections)
		{
			ss << "[" << section.first << "]" << std::endl;

			for (const auto& parameter : section.second.values)
			{
				ss << parameter.first << " = " << parameter.second << std::endl;
			}

			ss << std::endl;
		}

		// Write buffer to file
		std::ofstream os(_filename, std::ios_base::binary);

		if (!os.is_open())
		{
			return write_result::open_file_failure;
		}

		os.write(ss.str().c_str(), ss.str().size());
		os.close();
		return write_result::success;
	}

}