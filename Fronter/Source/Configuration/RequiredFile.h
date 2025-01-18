#ifndef CONFIGURATION_REQUIRED_FILE
#define CONFIGURATION_REQUIRED_FILE


#include <Parser.h>
#include <filesystem>



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
	[[nodiscard]] const auto& getValue() const { return value; }
	[[nodiscard]] auto isMandatory() const { return mandatory; }
	[[nodiscard]] auto isOutputtable() const { return outputtable; }
	[[nodiscard]] auto getID() const { return ID; }

	void setID(const int theID) { ID = theID; }
	void setValue(const std::filesystem::path& theValue) { value = theValue; }

  private:
	void registerKeys();

	bool mandatory = false;
	bool outputtable = false;
	int ID = 0;
	std::string name;
	std::string tooltip;
	std::string displayName;
	std::string searchPathType;
	std::filesystem::path searchPath;
	std::filesystem::path fileName;
	std::string allowedExtension;
	std::filesystem::path value;
};

#endif // CONFIGURATION_REQUIRED_FILE
