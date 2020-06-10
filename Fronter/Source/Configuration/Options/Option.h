#ifndef CONFIGURATION_OPTION
#define CONFIGURATION_OPTION
#include "CheckBoxSelector.h"
#include "Parser.h"
#include "RadioSelector.h"
#include "TextSelector.h"

class Option: commonItems::parser
{
  public:
	Option() = default;
	explicit Option(std::istream& theStream, int theID);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getTooltip() const { return tooltip; }
	[[nodiscard]] const auto& getRadioSelector() const { return radioSelector; }
	[[nodiscard]] const auto& getTextSelector() const { return textSelector; }
	[[nodiscard]] const auto& getCheckBoxSelector() const { return checkBoxSelector; }
	[[nodiscard]] auto getID() const { return ID; }
	[[nodiscard]] bool isCheckBoxSelectorPreloaded() const;

	[[nodiscard]] std::string getValue() const;
	[[nodiscard]] std::set<std::string> getValues() const;

	void setRadioSelectorID(int selection) const;
	void setRadioSelectorValue(const std::string& selection) const;
	void setCheckBoxSelectorIDs(const std::set<int>& selection) const;
	void setCheckBoxSelectorValues(const std::set<std::string>& selection) const;
	void setTextSelectorValue(const std::string& selection) const;
	void setValue(const std::string& selection) const;
	void setValue(const std::set<std::string>& selection) const;
	void setCheckBoxSelectorPreloaded() const;

	void insertCheckBoxSelector(std::shared_ptr<CheckBoxSelector> theSelector) { checkBoxSelector = std::pair(true, std::move(theSelector)); }

  private:
	void registerKeys();

	int ID = 0;
	std::string name;
	std::string tooltip;
	std::string displayName;
	std::pair<bool, std::shared_ptr<RadioSelector>> radioSelector;			// enabled, selector.
	std::pair<bool, std::shared_ptr<TextSelector>> textSelector;			// enabled, selector.
	std::pair<bool, std::shared_ptr<CheckBoxSelector>> checkBoxSelector; // enabled, selector.
};

#endif // CONFIGURATION_OPTION
