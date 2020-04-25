#include "RadioSelector.h"
#include "Log.h"
#include "ParserHelpers.h"

Configuration::RadioSelector::RadioSelector(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void Configuration::RadioSelector::registerKeys()
{
	registerKeyword("radioOption", [this](const std::string& unused, std::istream& theStream) {
		optionCounter++;
		auto newOption = std::make_shared<RadioOption>(theStream, optionCounter);
		radioOptions.emplace_back(newOption);
	});
	registerRegex("[A-Za-z0-9:_\\.-]+", commonItems::ignoreItem);
}

std::string Configuration::RadioSelector::getSelectedValue() const
{
	for (const auto& option: radioOptions)
		if (option->getValue())
			return option->getName();
	return std::string();
}
