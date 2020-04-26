#ifndef CONFIGURATION_OPTION
#define CONFIGURATION_OPTION
#include "newParser.h"
#include "RadioSelector.h"
#include "TextSelector.h"

namespace Configuration
{
class Option: commonItems::parser
{
  public:
	Option() = default;
	explicit Option(std::istream& theStream, int theID);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getTooltip() const { return tooltip; }
	[[nodiscard]] std::string getValue() const;
	[[nodiscard]] auto getID() const { return ID; }
	[[nodiscard]] const auto& getRadioSelector() const { return radioSelector; }
	[[nodiscard]] const auto& getTextSelector() const { return textSelector; }

	void setRadioSelectorValue(int selection) const;
	void setTextSelectorValue(const std::string& selection) const;
	void setValue(const std::string& selection) const;

  private:
	void registerKeys();
	std::string name;
	std::string tooltip;
	std::string displayName;
	std::pair<bool, std::shared_ptr<RadioSelector>> radioSelector; // enabled, selector.
	std::pair<bool, std::shared_ptr<TextSelector>> textSelector; // enabled, selector.
	int ID = 0;
};
} // namespace Configuration

#endif // CONFIGURATION_OPTION
