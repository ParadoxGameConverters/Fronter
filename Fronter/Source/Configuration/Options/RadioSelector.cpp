#include "RadioSelector.h"
#include "CommonRegexes.h"
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
	registerKeyword("radioOption", [this](std::istream& theStream) {
		optionCounter++;
		auto newOption = std::make_shared<RadioOption>(theStream, optionCounter);
		radioOptions.emplace_back(newOption);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
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

void RadioSelector::setSelectedID(int selection)
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
		Log(LogLevel::Warning) << "Attempted setting a radio selector ID that does not exist!";
}

void RadioSelector::setSelectedValue(const std::string& selection)
{
	auto isSet = false;
	for (const auto& option: radioOptions)
	{
		if (option->getName() == selection)
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