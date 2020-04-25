#ifndef CONFIGURATION_RADIO_SELECTOR
#define CONFIGURATION_RADIO_SELECTOR
#include "newParser.h"
#include "RadioOption.h"

namespace Configuration
{
class RadioSelector: commonItems::parser
{
  public:
	RadioSelector() = default;
	explicit RadioSelector(std::istream& theStream);

	[[nodiscard]] const auto& getOptions() const { return radioOptions; }
	[[nodiscard]] auto getID() const { return ID; }
	void setID(int theID) { ID = theID; }
	[[nodiscard]] std::string getSelectedValue() const;
	
  private:
	void registerKeys();
	int ID = 0;
	int optionCounter = 0;
	std::vector<std::shared_ptr<RadioOption>> radioOptions;
};
} // namespace Configuration

#endif // CONFIGURATION_RADIO_SELECTOR
