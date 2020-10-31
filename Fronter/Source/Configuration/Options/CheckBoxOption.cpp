#include "CheckBoxOption.h"
#include "Log.h"
#include "ParserHelpers.h"

CheckBoxOption::CheckBoxOption(std::istream& theStream, int theID): ID(theID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CheckBoxOption::registerKeys()
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
	registerKeyword("default", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString defaultStr(theStream);
		defaulted = defaultStr.getString() == "true";
		if (defaulted)
			value = true;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
