#ifndef CONFIGURATION_REQUIRED_FILE
#define CONFIGURATION_REQUIRED_FILE
#include "newParser.h"

namespace Configuration
{
class RequiredFile: commonItems::parser
{
  public:
	RequiredFile() = default;
	explicit RequiredFile(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getSearchPathType() const { return searchPathType; }
	[[nodiscard]] const auto& getSearchPath() const { return searchPath; }
	[[nodiscard]] const auto& getTooltip() const { return tooltip; }
	[[nodiscard]] const auto& getFilename() const { return fileName; }
	[[nodiscard]] const auto& getAllowedExtension() const { return allowedExtension; }
	[[nodiscard]] auto isMandatory() const { return mandatory; }
	[[nodiscard]] auto isOutputtable() const { return outputtable; }
	[[nodiscard]] auto getID() const { return ID; }
	void setID(int theID) { ID = theID; }
	void setValue(const std::string& theValue) { value = theValue; }
	
  private:
	void registerKeys();
	std::string name;
	std::string tooltip;
	std::string displayName;
	bool mandatory = false;
	bool outputtable = false;
	std::string searchPathType;
	std::string searchPath;
	std::string fileName;
	std::string allowedExtension;
	int ID = 0;
	std::string value;
};
} // namespace Configuration

#endif // CONFIGURATION_REQUIRED_FILE
