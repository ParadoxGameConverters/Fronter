#include "RadioSelector.h"
#include "Log.h"
#include "ParserHelpers.h"

RadioSelector::RadioSelector(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void RadioSelector::registerKeys()
{
	registerKeyword("radioOption", [this](const std::string& unused, std::istream& theStream) {
		optionCounter++;
		auto newOption = std::make_shared<RadioOption>(theStream, optionCounter);
		radioOptions.emplace_back(newOption);
	});
	registerRegex("[A-Za-z0-9:_\\.-]+", commonItems::ignoreItem);
}

std::string RadioSelector::getSelectedValue() const
{
	for (const auto& option: radioOptions)
		if (option->getValue())
			return option->getName();
	return std::string();
}

int RadioSelector::getSelectedID() const
{
	for (const auto& option: radioOptions)
		if (option->getValue())
			return option->getID();
	return 0;
}

void RadioSelector::setSelectedValue(int selection)
{
	auto isSet = false;
	for (const auto& option: radioOptions)
	{
		if (option->getID() == selection)
		{
			option->setValue();
			isSet = true;
		}
		else
			option->unsetValue();
	}
	if (!isSet)
		Log(LogLevel::Warning) << "Attempted setting a radio selector value that does not exist!";
}