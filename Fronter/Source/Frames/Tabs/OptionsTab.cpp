#include "OptionsTab.h"
#include "OSCompatibilityLayer.h"
#include <wx/wrapsizer.h>
#include "OptionBox.h"

OptionsTab::OptionsTab(wxWindow* parent): wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
}

void OptionsTab::initializeOptions()
{
	wxWrapSizer* optionsTabSizer = new wxWrapSizer(wxHORIZONTAL);
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
	
	GetParent()->Layout();
}
