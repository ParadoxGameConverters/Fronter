#include "TextSelector.h"
#include "Log.h"
#include "ParserHelpers.h"

TextSelector::TextSelector(std::istream& theStream, std::string language): setLanguage(std::move(language))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void TextSelector::registerKeys()
{
	registerKeyword("editable", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString editStr(theStream);
		editable = editStr.getString() == "true";
	});
	registerKeyword("value", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString valueStr(theStream);
		value = valueStr.getString();
	});
	registerKeyword("tooltip_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString tooltipStr(theStream);
		tooltip = tooltipStr.getString();
	});
	registerRegex("[A-Za-z0-9:_\\.-]+", commonItems::ignoreItem);
}
