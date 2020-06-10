#include "OptionsTab.h"
#include "OSCompatibilityLayer.h"
#include <wx/wrapsizer.h>

OptionsTab::OptionsTab(wxWindow* parent): wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
}

void OptionsTab::initializeOptions()
{
	optionsTabSizer = new wxWrapSizer(wxHORIZONTAL);
	SetScrollRate(0, 16);
	SetSizer(optionsTabSizer);

	// Load options from configuration and initialize option boxes to be inserted into sizer.
	for (const auto option: configuration->getOptions())
	{
		OptionBox* newOption = new OptionBox(this, option->getName(), option);
		newOption->loadLocalization(localization);
		newOption->initializeOption();
		optionsTabSizer->Add(newOption);
	}

	// For mods we make a special "option" that we can alter at will. It will be hidden unless there are actual mods found and required.
	if (!configuration->getAutoLocatedMods().empty()) // If mods path is unset, we'll create it later once it's set.
	{
		fillModOption();
		createModOptionBox();
	}
	
	GetParent()->Layout();
}

void OptionsTab::fillModOption()
{
	Log(LogLevel::Debug) << "filling modoption";
	// We're manually filling the Option instead of relying on config files.
	auto modSelector = std::make_shared<CheckBoxSelector>();
	auto counter = 0;
	for (const auto& modName: configuration->getAutoLocatedMods() )
	{
		Log(LogLevel::Debug) << "Creating option " << modName << " counter " << counter;
		const auto newOption = std::make_shared<CheckBoxOption>(modName, counter);
		Log(LogLevel::Debug) << "Created option " << newOption->getName();
		++counter;
		Log(LogLevel::Debug) << "Inserting option " << modName << " counter " << counter;
		modSelector->insertOption(newOption);
	}
	Log(LogLevel::Debug) << "inserting modSelector";
	if (!modOption)
		modOption = std::make_shared<Option>();
	modOption->insertCheckBoxSelector(modSelector);	
	Log(LogLevel::Debug) << "filling modoption inserted";
}


void OptionsTab::createModOptionBox()
{
	Log(LogLevel::Debug) << "creating modoptionbox";
	modOptionBox = new OptionBox(this, "selectedMods", modOption);
	Log(LogLevel::Debug) << "initializing modoptionbox";
	modOptionBox->loadLocalization(localization); // Need to load it to prevent the pointer be left dangling.
	modOptionBox->overrideLocalization();
	modOptionBox->initializeOption();
	Log(LogLevel::Debug) << "adding to sizer";
	optionsTabSizer->Add(modOptionBox);
	Log(LogLevel::Debug) << "creating modoptionbox done";
}

