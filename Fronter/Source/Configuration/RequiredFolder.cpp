#include "RequiredFolder.h"
#include "CommonRegexes.h"
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
	registerKeyword("name", [this](std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("tooltip", [this](std::istream& theStream) {
		const commonItems::singleString tooltipStr(theStream);
		tooltip = tooltipStr.getString();
	});
	registerKeyword("displayName", [this](std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		displayName = nameStr.getString();
	});
	registerKeyword("mandatory", [this](std::istream& theStream) {
		const commonItems::singleString mandatoryStr(theStream);
		mandatory = mandatoryStr.getString() == "true";
	});
	registerKeyword("outputtable", [this](std::istream& theStream) {
		const commonItems::singleString outputtableStr(theStream);
		outputtable = outputtableStr.getString() == "true";
	});
	registerKeyword("searchPathType", [this](std::istream& theStream) {
		const commonItems::singleString pathStr(theStream);
		searchPathType = pathStr.getString();
	});
	registerKeyword("searchPathID", [this](std::istream& theStream) {
		const commonItems::singleString pathStr(theStream);
		searchPathID = pathStr.getString();
	});
	registerKeyword("searchPath", [this](std::istream& theStream) {
		const commonItems::singleString pathStr(theStream);
		searchPath = pathStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
