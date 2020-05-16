#ifndef LOCALIZATION
#define LOCALIZATION
#include <map>
#include <string>
#include <vector>

class Localization
{
  public:
	Localization();
#if defined __WIN32__
	[[nodiscard]] std::wstring translate(const std::string& key);
	[[nodiscard]] std::wstring translateLanguage(const std::string& language);
#elif defined __linux
	[[nodiscard]] std::string translate(const std::string& key);
	[[nodiscard]] std::string translateLanguage(const std::string& language);
#endif // defined
	[[nodiscard]] const auto& getLoadedLanguages() const { return loadedLangauges; }
	[[nodiscard]] const auto& getSetLanguage() const { return setLanguage; }

	void saveLanguage(int ID);

  private:
	void loadLanguages();

	std::vector<std::string> loadedLangauges;
	std::map<std::string, std::string> languages;
	std::map<std::string, std::map<std::string, std::string>> translations; // key, map<language, text>
	std::string setLanguage = "english";
};

#endif // LOCALIZATION
