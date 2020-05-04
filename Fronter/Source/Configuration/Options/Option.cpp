#include "Option.h"
#include "Log.h"
#include "ParserHelpers.h"

Option::Option(std::istream& theStream, int theID, std::string language): ID(theID), setLanguage(std::move(language))
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
	registerKeyword("tooltip_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString tooltipStr(theStream);
		tooltip = tooltipStr.getString();
	});
	registerKeyword("displayName_" + setLanguage, [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameStr(theStream);
		displayName = nameStr.getString();
	});
	registerKeyword("radioSelector", [this](const std::string& unused, std::istream& theStream) {
		auto newSelector = std::make_shared<RadioSelector>(theStream, setLanguage);
		radioSelector = std::pair(true, newSelector);
	});
	registerKeyword("textSelector", [this](const std::string& unused, std::istream& theStream) {
		auto newSelector = std::make_shared<TextSelector>(theStream, setLanguage);
		textSelector = std::pair(true, newSelector);
	});
	registerRegex("[A-Za-z0-9:_\\.-]+", commonItems::ignoreItem);
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

void Option::setRadioSelectorID(int selection) const
{
	if (!radioSelector.first)
	{
		Log(LogLevel::Warning) << "Attempted setting a radio control in unknown radio option!";
		return;
	}
	radioSelector.second->setSelectedID(selection);
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

void Option::setValue(const std::string& selection) const
{
	if (textSelector.first)
		textSelector.second->setValue(selection);
	if (radioSelector.first)
		radioSelector.second->setSelectedValue(selection);
}
