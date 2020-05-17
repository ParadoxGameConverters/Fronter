#ifndef LOCALIZATION
#define LOCALIZATION
#include <map>
#include <string>
#include <vector>

class Localization
{
  public:
	Localization();
	[[nodiscard]] std::wstring translate(const std::string& key);
	[[nodiscard]] std::wstring translateLanguage(const std::string& language);
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
