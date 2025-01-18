#ifndef CONFIGURATION_REQUIRED_FOLDER
#define CONFIGURATION_REQUIRED_FOLDER



#include <Parser.h>
#include <filesystem>



class RequiredFolder: commonItems::parser
{
  public:
	RequiredFolder() = default;
	explicit RequiredFolder(std::istream& theStream);

	void setID(const int theID) { ID = theID; }
	void setValue(const std::filesystem::path& theValue) { value = theValue; }

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getDisplayName() const { return displayName; }
	[[nodiscard]] const auto& getSearchPathType() const { return searchPathType; }
	[[nodiscard]] const auto& getSearchPath() const { return searchPath; }
	[[nodiscard]] const auto& getTooltip() const { return tooltip; }
	[[nodiscard]] const auto& getValue() const { return value; }
	[[nodiscard]] const auto& getSearchPathID() const { return searchPathID; }
	[[nodiscard]] auto isMandatory() const { return mandatory; }
	[[nodiscard]] auto isOutputtable() const { return outputtable; }
	[[nodiscard]] auto getID() const { return ID; }

  private:
	void registerKeys();

	bool mandatory = false;
	bool outputtable = true; // If we have folders listed, they are generally required. Override with false in conf file.
	int ID = 0;

	std::string name;
	std::string tooltip;
	std::string displayName;
	std::string searchPathType;
	std::filesystem::path searchPath;
	std::string searchPathID;
	std::filesystem::path value;
};

#endif // CONFIGURATION_REQUIRED_FOLDER
