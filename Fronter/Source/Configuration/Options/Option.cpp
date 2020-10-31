#include "Option.h"
#include "Log.h"
#include "ParserHelpers.h"

Option::Option(std::istream& theStream, int theID): ID(theID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void Option::registerKeys()
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
	registerKeyword("textSelector", [this](const std::string& unused, std::istream& theStream) {
		auto newSelector = std::make_shared<TextSelector>(theStream);
		textSelector = std::pair(true, newSelector);
	});
	registerKeyword("checkBoxSelector", [this](const std::string& unused, std::istream& theStream) {
		auto newSelector = std::make_shared<CheckBoxSelector>(theStream);
		checkBoxSelector = std::pair(true, newSelector);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void Option::setRadioSelectorValue(const std::string& selection) const
{
	if (!radioSelector.first)
	{
		Log(LogLevel::Warning) << "Attempted setting a radio control in unknown radio option!";
		return;
	}
	radioSelector.second->setSelectedValue(selection);
}

void Option::setCheckBoxSelectorValues(const std::set<std::string>& selection) const
{
	if (!checkBoxSelector.first)
	{
		Log(LogLevel::Warning) << "Attempted setting a checkbox control in unknown checkbox option!";
		return;
	}
	checkBoxSelector.second->setSelectedValues(selection);
}

void Option::setRadioSelectorID(int selection) const
{
	if (!radioSelector.first)
	{
		Log(LogLevel::Warning) << "Attempted setting a radio control in unknown radio option!";
		return;
	}
	radioSelector.second->setSelectedID(selection);
}

void Option::setCheckBoxSelectorIDs(const std::set<int>& selection) const
{
	if (!checkBoxSelector.first)
	{
		Log(LogLevel::Warning) << "Attempted setting a checkbox control in unknown checkbox option!";
		return;
	}
	checkBoxSelector.second->setSelectedIDs(selection);
}

void Option::setTextSelectorValue(const std::string& selection) const
{
	if (!textSelector.first)
	{
		Log(LogLevel::Warning) << "Attempted setting a text control which does not exist!";
		return;
	}
	textSelector.second->setValue(selection);
}

std::string Option::getValue() const
{
	if (radioSelector.first)
	{
		return radioSelector.second->getSelectedValue();
	}
	if (textSelector.first)
	{
		return textSelector.second->getValue();
	}

	return std::string();
}

std::set<std::string> Option::getValues() const
{
	if (checkBoxSelector.first)
	{
		return checkBoxSelector.second->getSelectedValues();
	}
	return std::set<std::string>();
}

void Option::setValue(const std::string& selection) const
{
	if (textSelector.first)
		textSelector.second->setValue(selection);
	if (radioSelector.first)
		radioSelector.second->setSelectedValue(selection);
}

void Option::setValue(const std::set<std::string>& selection) const
{
	if (checkBoxSelector.first)
		checkBoxSelector.second->setSelectedValues(selection);
}

bool Option::isCheckBoxSelectorPreloaded() const
{
	if (!checkBoxSelector.first || !checkBoxSelector.second)
		return false;
	return checkBoxSelector.second->isPreloaded();
}

void Option::setCheckBoxSelectorPreloaded() const
{
	if (!checkBoxSelector.first || !checkBoxSelector.second)
		return;
	checkBoxSelector.second->setPreloaded();
}
