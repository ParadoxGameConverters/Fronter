#include "Localization.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <fstream>
#include <regex>
namespace fs = std::filesystem;

Localization::Localization()
{
	if (!fs::exists("Configuration/converter_languages.yml"))
	{
		Log(LogLevel::Error) << "No localization found!";
		return;
	}

	std::ifstream langfile("Configuration/converter_languages.yml");
	std::string line;
	while (std::getline(langfile, line))
	{
		auto pos = line.find_first_of(':');
		auto language = line.substr(2, pos - 2);
		pos = line.find_first_of('\"');
		const auto secpos = line.find_last_of('\"');
		auto langtext = line.substr(pos + 1, secpos - pos - 1);
		languages.insert(std::pair(language, langtext));
	}
	langfile.close();
	loadLanguages();

	if (fs::exists("Configuration/fronter-language.txt"))
	{
		std::ifstream userFile("Configuration/fronter-language.txt");
		std::getline(userFile, line);
		if (line.find("language=") == 0)
		{
			setLanguage = line.substr(9, line.length());
		}
		userFile.close();
	}
}

void Localization::loadLanguages()
{
	for (const auto& language: languages)
	{
		if (!fs::exists("Configuration/converter_l_" + language.first + ".yml"))
		{
			Log(LogLevel::Error) << "Configuration/converter_l_" + language.first + ".yml not found !";
			continue;
		}
		std::ifstream langfile("Configuration/converter_l_" + language.first + ".yml");
		std::string line;
		std::getline(langfile, line);
		if (line.find("l_" + language.first + ":") != 0)
		{
			Log(LogLevel::Error) << "Configuration/converter_l_" + language.first + ".yml holds wrong language!";
			langfile.close();
			continue;
		}
		while (std::getline(langfile, line))
		{
			auto pos = line.find_first_of(':');
			auto key = line.substr(1, pos - 1);
			pos = line.find_first_of('\"');
			const auto secpos = line.find_last_of('\"');
			auto text = line.substr(pos + 1, secpos - pos - 1);
			translations[key].insert(std::pair(language.first, text));
		}
		langfile.close();
		loadedLangauges.emplace_back(language.first);
	}
}

std::wstring Localization::translate(const std::string& key)
{
	std::string toReturn;
	if (!translations.count(key))
	{
		return std::wstring();
	}
	if (translations[key].count(setLanguage))
		toReturn = translations[key][setLanguage];
	else if (translations[key].count("english"))
		toReturn = translations[key]["english"];
	else
		return std::wstring();

	toReturn = std::regex_replace(toReturn, std::regex(R"(\\n)"), "\n");
	return Utils::convertUTF8ToUTF16(toReturn);
}

std::wstring Localization::translateLanguage(const std::string& language)
{
	if (!languages.count(language))
	{
		return std::wstring();
	}
	return Utils::convertUTF8ToUTF16(languages[language]);
}

void Localization::saveLanguage(int ID)
{
	if (ID > static_cast<int>(loadedLangauges.size()) + 1)
		return;
	setLanguage = loadedLangauges[ID];
	std::ofstream langfile("Configuration/fronter-language.txt");
	if (langfile.is_open())
	{
		langfile << "language=" << loadedLangauges[ID];
	}
	langfile.close();
}
