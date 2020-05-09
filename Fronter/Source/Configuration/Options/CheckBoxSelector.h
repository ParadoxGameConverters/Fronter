#ifndef CONFIGURATION_CHECKBOX_SELECTOR
#define CONFIGURATION_CHECKBOX_SELECTOR
#include "CheckBoxOption.h"
#include "Parser.h"
#include <set>

class CheckBoxSelector: commonItems::parser
{
  public:
	CheckBoxSelector() = default;
	explicit CheckBoxSelector(std::istream& theStream);

	[[nodiscard]] const auto& getOptions() const { return checkBoxOptions; }
	[[nodiscard]] auto getID() const { return ID; }
	[[nodiscard]] auto isPreloaded() const { return preloaded; }

	[[nodiscard]] std::set<std::string> getSelectedValues() const;
	[[nodiscard]] std::set<int> getSelectedIDs() const;

	void setID(int theID) { ID = theID; }
	void setSelectedIDs(const std::set<int>& selection);
	void setSelectedValues(const std::set<std::string>& selection);
	void setPreloaded() { preloaded = true; }

  private:
	void registerKeys();
	int ID = 0;
	int optionCounter = 0;
	bool preloaded = false;
	std::vector<std::shared_ptr<CheckBoxOption>> checkBoxOptions;
};

#endif // CONFIGURATION_CHECKBOX_SELECTOR
