#include "TextSelector.h"
#include "Log.h"
#include "ParserHelpers.h"

Configuration::TextSelector::TextSelector(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void Configuration::TextSelector::registerKeys()
{
	registerKeyword("editable", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString editStr(theStream);
		editable = editStr.getString() == "true";		
	});
	registerKeyword("value", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString valueStr(theStream);
		value = valueStr.getString();
	});	
	registerKeyword("tooltip", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString tooltipStr(theStream);
		tooltip = tooltipStr.getString();
	});
	registerRegex("[A-Za-z0-9:_\\.-]+", commonItems::ignoreItem);
}
