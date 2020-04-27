#include "RequiredFolder.h"
#include "Log.h"
#include "ParserHelpers.h"

RequiredFolder::RequiredFolder(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void RequiredFolder::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("tooltip", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString tooltipStr(theStream);
		tooltip = tooltipStr.getString();
	});
	registerKeyword("displayName", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		displayName = nameStr.getString();
	});
	registerKeyword("mandatory", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString mandatoryStr(theStream);
		mandatory = mandatoryStr.getString() == "true";
	});
	registerKeyword("outputtable", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString outputtableStr(theStream);
		outputtable = outputtableStr.getString() == "true";
	});
	registerKeyword("searchPathType", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString pathStr(theStream);
		searchPathType = pathStr.getString();
	});
	registerKeyword("searchPathID", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString pathStr(theStream);
		searchPathID = pathStr.getString();
	});
	registerKeyword("searchPath", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString pathStr(theStream);
		searchPath = pathStr.getString();
	});
	registerRegex("[A-Za-z0-9\\:_.-]+", commonItems::ignoreItem);
}
