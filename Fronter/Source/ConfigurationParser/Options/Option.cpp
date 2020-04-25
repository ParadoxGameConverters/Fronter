#include "Option.h"
#include "Log.h"
#include "ParserHelpers.h"

Configuration::Option::Option(std::istream& theStream, int theID): ID(theID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void Configuration::Option::registerKeys()
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
	registerKeyword("radioSelector", [this](const std::string& unused, std::istream& theStream) {
		auto newSelector = std::make_shared<RadioSelector>(theStream);
		radioSelector = std::pair(true, newSelector);
	});
	registerRegex("[A-Za-z0-9:_\\.-]+", commonItems::ignoreItem);
}

void Configuration::Option::setRadioSelectorValue(int selection) const
{
	if (!radioSelector.first)
	{
		Log(LogLevel::Warning) << "Attempted setting a radio control in unknown radio option!";
		return;
	}
	radioSelector.second->setSelectedValue(selection);
}

std::string Configuration::Option::getValue() const
{
	if (radioSelector.first)
	{
		return radioSelector.second->getSelectedValue();
	}

	return std::string();
}