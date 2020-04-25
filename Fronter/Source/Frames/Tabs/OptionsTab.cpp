#include "OptionsTab.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "OptionBox.h"
#include <wx/wrapsizer.h>

OptionsTab::OptionsTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
}

void OptionsTab::initializeOptions()
{
	// Initialize a wrap horizontal sizer
	wxWrapSizer* optionsSizer = new wxWrapSizer(wxHORIZONTAL,wxBORDER_SIMPLE);
	SetSizer(optionsSizer);

	// Load options from configuration and initialize option boxes to be inserted into sizer.
	for (const auto option: configuration->getOptions())
	{
		OptionBox* newOption = new OptionBox(this, option->getName(), option);		
		newOption->initializeOption();
		optionsSizer->Add(newOption);
	}
	Layout();
}
