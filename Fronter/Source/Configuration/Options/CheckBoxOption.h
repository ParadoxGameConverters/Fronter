#ifndef CONFIGURATION_CHECKBOX_OPTION
#define CONFIGURATION_CHECKBOX_OPTION
#include "Parser.h"

class CheckBoxOption: commonItems::parser
{
  public:
	CheckBoxOption() = default;
	explicit CheckBoxOption(std::istream& theStream, int theID);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getTooltip() const { return tooltip; }
	[[nodiscard]] auto getValue() const { return value; }
	[[nodiscard]] auto isDefault() const { return defaulted; }
	[[nodiscard]] auto getID() const { return ID; }

	void setValue() { value = true; }
	void unsetValue() { value = false; }

  private:
	void registerKeys();

	bool defaulted = false;
	bool value = false;
	int ID = 0;
	std::string name;
	std::string tooltip;
	std::string displayName;
};

#endif // CONFIGURATION_CHECKBOX_OPTION
