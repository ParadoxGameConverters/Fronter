#include "OptionBox.h"
#include "../../ConfigurationParser/Options/Option.h"
#include "Log.h"
#include <wx/filepicker.h>

OptionBox::OptionBox(wxWindow* parent, std::shared_ptr<Configuration::Option> theOption):
	 optionName(theOption->getName()), option(theOption), wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME | wxEXPAND)
{
}

void OptionBox::initializeOption()
{
	SetMaxSize(wxSize(350, 250));

	flexGridSizer = new wxFlexGridSizer(1, 2, 2);
	SetSizer(flexGridSizer);

	wxStaticText* st = new wxStaticText(this, wxID_ANY, option->getDisplayName(), wxDefaultPosition, wxDefaultSize);
	st->SetSize(wxSize(st->GetSize().GetWidth() + 10, st->GetSize().GetHeight() + 10));
	st->SetToolTip(option->getTooltip());
	flexGridSizer->Add(st);
	SetMinSize(wxSize(st->GetSize().GetWidth(), 150));

	if (option->getRadioSelector().first)
	{
		auto first = true;
		for (const auto& radioOption: option->getRadioSelector().second->getOptions())
		{
			wxRadioButton* theButton;
			if (first)
			{
				theButton = new wxRadioButton(this, radioOption->getID(), radioOption->getDisplayName(), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
				first = false;
			}
			else
			{
				theButton = new wxRadioButton(this, radioOption->getID(), radioOption->getDisplayName(), wxDefaultPosition, wxDefaultSize);
			}
			theButton->SetToolTip(radioOption->getTooltip());
			theButton->SetValue(radioOption->isDefault());
			
			theButton->Bind(wxEVT_RADIOBUTTON, [this](wxCommandEvent& event) {
				auto id = event.GetId();
				Log(LogLevel::Debug) << optionName << " selected: " << id;
				option->setRadioSelectorValue(id);
			});
			
			flexGridSizer->Add(theButton);
			SetMinSize(wxSize(std::max(theButton->GetSize().GetWidth(), GetMinWidth()), 150));
		}
	}



	Layout();
}

wxBEGIN_EVENT_TABLE(OptionBox, wxWindow)
wxEND_EVENT_TABLE()
