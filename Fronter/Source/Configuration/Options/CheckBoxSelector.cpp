#include "CheckBoxSelector.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

CheckBoxSelector::CheckBoxSelector(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void CheckBoxSelector::registerKeys()
{
	registerKeyword("checkBoxOption", [this](std::istream& theStream) {
		optionCounter++;
		auto newOption = std::make_shared<CheckBoxOption>(theStream, optionCounter);
		checkBoxOptions.emplace_back(newOption);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::set<std::string> CheckBoxSelector::getSelectedValues() const
{
	std::set<std::string> toReturn;
	for (const auto& option: checkBoxOptions)
		if (option->getValue())
			toReturn.insert(option->getName());
	return toReturn;
}

std::set<int> CheckBoxSelector::getSelectedIDs() const
{
	std::set<int> toReturn;
	for (const auto& option: checkBoxOptions)
		if (option->getValue())
			toReturn.insert(option->getID());
	return toReturn;
}

void CheckBoxSelector::setSelectedIDs(const std::set<int>& selection)
{
	for (const auto& option: checkBoxOptions)
	{
		if (selection.count(option->getID()))
			option->setValue();
		else
			option->unsetValue();
	}
}

void CheckBoxSelector::setSelectedValues(const std::set<std::string>& selection)
{
	for (const auto& option: checkBoxOptions)
	{
		if (selection.count(option->getName()))
			option->setValue();
		else
			option->unsetValue();
	}
}