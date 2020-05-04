#include "RadioOption.h"
#include "Log.h"
#include "ParserHelpers.h"

RadioOption::RadioOption(std::istream& theStream, int theID, std::string language): ID(theID), setLanguage(std::move(language))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void RadioOption::registerKeys()
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		name = nameStr.getString();
	});
	registerKeyword("tooltip_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString tooltipStr(theStream);
		tooltip = tooltipStr.getString();
	});
	registerKeyword("displayName_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		displayName = nameStr.getString();
	});
	registerKeyword("default", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString defaultStr(theStream);
		defaulted = defaultStr.getString() == "true";
		if (defaulted)
			value = true;
	});
	registerRegex("[A-Za-z0-9:_\\.-]+", commonItems::ignoreItem);
}
