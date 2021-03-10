#include "TextSelector.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

TextSelector::TextSelector(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void TextSelector::registerKeys()
{
	registerKeyword("editable", [this](std::istream& theStream) {
		const commonItems::singleString editStr(theStream);
		editable = editStr.getString() == "true";
	});
	registerKeyword("value", [this](std::istream& theStream) {
		const commonItems::singleString valueStr(theStream);
		value = valueStr.getString();
	});
	registerKeyword("tooltip", [this](std::istream& theStream) {
		const commonItems::singleString tooltipStr(theStream);
		tooltip = tooltipStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
