#include "config.hpp"
#include <sys/stat.h>
#include <exception>
#include <fstream>
#include <sstream>

using namespace std;
using field = pair<string, string>;

Config::Config(string path)
{
	struct stat buffer;
	bool file_exists = (stat(path.c_str(), &buffer) == 0);
	
	if (!file_exists) { throw exception("File does not exist in the given path."); }
	else {
		this->parse_config(path);
	}
}

void Config::parse_config(string path)
{
	struct stat buffer;
	bool file_exists = (stat(path.c_str(), &buffer) == 0);

	if (!file_exists) { throw exception("File does not exist in the given path."); }
	else {
		ifstream FILE;
		FILE.open(path);

		if (!FILE.is_open()) { throw exception("File could not be opened."); }

		string line_buffer, key, value;;
		char ignore_char;
		size_t current_pos = 0;

		//getline(FILE, line_buffer);

		while (getline(FILE, line_buffer))
		{
			if (line_buffer == "") { continue; }
			else{
				if ((line_buffer.front() == '[') && (line_buffer.back() == ']'))
				{
					line_buffer.erase(0, 1);
					line_buffer.erase(line_buffer.size() - 1);
					this->m_config.push_back(section(line_buffer));
					current_pos = m_config.size() - 1;
					section_map[line_buffer] = current_pos;
				}

				else
				{
					stringstream buffer_stream(line_buffer);
					buffer_stream >> key >> ignore_char >> value;
					this->m_config[current_pos].sec_data.push_back(field(key, value));
				}
			}
		}

		FILE.close();
	}
}

std::string Config::get_value(string section_name, string key_name)
{
	size_t section_pos;
	try {
		section_pos = section_map.at(section_name);
	}
	catch(const std::out_of_range& err) {
		throw exception("Section does not exist.");
	}

	if (this->m_config[section_pos].sec_name == section_name) {
		for (const auto& [k, v] : this->m_config[section_pos].sec_data)
		{
			if (k == key_name) { return v; }
		}

		throw exception("Key doesn't exist.");
	}
	
	else
	{
		throw exception("Section doesn't exist.");
	}
}

bool Config::set_value(string section_name, string key_name, string value_name)
{
	size_t section_pos;
	try {
		section_pos = section_map.at(section_name);
	}
	catch (const std::out_of_range& err) {
		throw exception("Section does not exist.");
	}

	if (this->m_config[section_pos].sec_name == section_name) {
		for (auto& [k, v] : this->m_config[section_pos].sec_data)
		{
			if (k == key_name) { v = value_name; return true; }
		}

		throw exception("Key doesn't exist.");
	}

	else
	{
		throw exception("Section doesn't exist.");
	}
}

void Config::new_section(string sname)
{
	this->m_config.push_back(section(sname));
	section_map[sname] = m_config.size() - 1;
}

void Config::new_key(string sname, string key, string value)
{
	size_t section_pos;
	try {
		section_pos = section_map.at(sname);
	}
	catch (const std::out_of_range& err) {
		throw exception("Section does not exist.");
	}

	this->m_config[section_pos].sec_data.push_back(field(key, value));
}

vector<string> Config::section_list()
{
	size_t len = this->m_config.size();
	vector<string> sec_list(len);
	for (size_t i = 0; i < len; i++)
	{
		sec_list[i] = this->m_config[i].sec_name;
	}
	return sec_list;
}

vector<string> Config::key_list(string sname)
{
	size_t section_pos, len;
	try {
		section_pos = section_map.at(sname);
	}
	catch (const std::out_of_range& err) {
		throw exception("Section does not exist.");
	}
	len = this->m_config[section_pos].sec_data.size();
	vector<string> k_list(len);

	for (size_t i = 0; i < len; i++)
	{
		k_list[i] = this->m_config[section_pos].sec_data[i].first;
	}

	return k_list;
}

void Config::save_config(string path)
{
	ofstream FILE;
	FILE.open(path);
	vector<string> key_list;
	vector<string> section_list;
	string value;

	if (!FILE.is_open()) { throw exception("File could not be opened."); }
	else
	{
		section_list = this->section_list();

		for (const auto& section : section_list)
		{
			FILE << "[" << section << "]\n";
			key_list = this->key_list(section);
			for (const auto& key : key_list)
			{
				value = this->get_value(section, key);
				FILE << key << " = " << value << "\n";
			}
			FILE << "\n";
		}
	}
	FILE.close();
}
