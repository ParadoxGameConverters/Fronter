#ifndef CONFIGURATION_TEXT_SELECTOR
#define CONFIGURATION_TEXT_SELECTOR
#include "Parser.h"

class TextSelector: commonItems::parser
{
  public:
	TextSelector() = default;
	explicit TextSelector(std::istream& theStream, std::string language);

	[[nodiscard]] const auto& getValue() const { return value; }
	[[nodiscard]] const auto& getTooltip() const { return tooltip; }
	[[nodiscard]] auto getID() const { return ID; }
	[[nodiscard]] auto isEditable() const { return editable; }

	void setID(int theID) { ID = theID; }
	void setValue(const std::string& theValue) { value = theValue; }

  private:
	void registerKeys();

	bool editable = true; // editable unless disabled
	int ID = 0;
	std::string value;
	std::string tooltip;
	std::string setLanguage;
};

#endif // CONFIGURATION_TEXT_SELECTOR
