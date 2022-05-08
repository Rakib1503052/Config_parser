#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>

class Config {
private:
	struct section {
		std::string sec_name;
		std::vector<std::pair<std::string, std::string>> sec_data;
		section(std::string name)
		{
			this->sec_name = name;
			//this->sec_data = data;
		}
	};
	//std::string path;
	std::vector<section> m_config;
	std::map<std::string, size_t> section_map;

public:
	Config() { return; }
	Config(std::string);
	void parse_config(std::string);
	std::string get_value(std::string, std::string);
	bool set_value(std::string, std::string, std::string);
	void new_section(std::string);
	void new_key(std::string, std::string, std::string);
	std::vector<std::string> section_list();
	std::vector<std::string> key_list(std::string);
	void save_config(std::string);
};