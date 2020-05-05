#ifndef CONFIGURATION_RADIO_SELECTOR
#define CONFIGURATION_RADIO_SELECTOR
#include "RadioOption.h"
#include "Parser.h"

class RadioSelector: commonItems::parser
{
  public:
	RadioSelector() = default;
	explicit RadioSelector(std::istream& theStream);

	[[nodiscard]] const auto& getOptions() const { return radioOptions; }
	[[nodiscard]] auto getID() const { return ID; }

	[[nodiscard]] std::string getSelectedValue() const;
	[[nodiscard]] int getSelectedID() const;

	void setID(int theID) { ID = theID; }
	void setSelectedID(int selection);
	void setSelectedValue(const std::string& selection);

  private:
	void registerKeys();
	int ID = 0;
	int optionCounter = 0;
	std::vector<std::shared_ptr<RadioOption>> radioOptions;
};

#endif // CONFIGURATION_RADIO_SELECTOR
