#ifndef CONFIGURATION_REQUIRED_FOLDER
#define CONFIGURATION_REQUIRED_FOLDER
#include "newParser.h"

namespace Configuration
{
class RequiredFolder: commonItems::parser
{
  public:
	RequiredFolder() = default;
	explicit RequiredFolder(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getSearchPathType() const { return searchPathType; }
	[[nodiscard]] const auto& getSearchPath() const { return searchPath; }
	[[nodiscard]] const auto& getTooltip() const { return tooltip; }
	[[nodiscard]] auto isMandatory() const { return mandatory; }
	[[nodiscard]] auto isOutputtable() const { return outputtable; }
	[[nodiscard]] const auto& getSearchPathID() const { return searchPathID; }
	[[nodiscard]] auto getID() const { return ID; }
	void setID(int theID) { ID = theID; }
	void setValue(const std::string& theValue) { value = theValue; }

	
  private:
	void registerKeys();
	std::string name;
	std::string tooltip;
	std::string displayName;
	bool mandatory = false;
	bool outputtable = true; // If we have folders listed, they are generally required. Override with false in conf file.
	std::string searchPathType;
	std::string searchPath;
	std::string searchPathID;
	int ID = 0;
	std::string value;
};
} // namespace Configuration

#endif // CONFIGURATION_REQUIRED_FOLDER
