#include "OptionBox.h"
#include "../../ConfigurationParser/Options/Option.h"
#include "Log.h"
#include <wx/filepicker.h>
#include <wx/textctrl.h>

OptionBox::OptionBox(wxWindow* parent, const std::string& theName, std::shared_ptr<Configuration::Option> theOption):
	 wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME | wxEXPAND), optionName(theName)
{
	option = theOption;
}

void OptionBox::initializeOption()
{
	SetMaxSize(wxSize(350, 250));

	wxFlexGridSizer* flexGridSizer = new wxFlexGridSizer(1, 2, 2);
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
				Log(LogLevel::Debug) << optionName << " selected: " << event.GetId();
				option->setRadioSelectorValue(event.GetId());
			});

			flexGridSizer->Add(theButton);
			SetMinSize(wxSize(std::max(theButton->GetSize().GetWidth(), GetMinWidth()), 150));
		}
	}

	if (option->getTextSelector().first)
	{
		const auto& selector = option->getTextSelector().second;
		long flag;
		if (selector->isEditable())
		{
			flag = wxBORDER_DEFAULT;
		}
		else
		{
			flag = wxBORDER_DEFAULT | wxTE_READONLY;
		}
		wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, selector->getValue(), wxDefaultPosition, wxDefaultSize, flag);
		textField->SetToolTip(selector->getTooltip());		

		textField->Bind(wxEVT_TEXT, [this](wxCommandEvent& event) {
			Log(LogLevel::Debug) << optionName << " entered: " << event.GetString();
			option->setTextSelectorValue(event.GetString().ToStdString());
		});
		flexGridSizer->Add(textField);
		SetMinSize(wxSize(std::max(textField->GetSize().GetWidth(), GetMinWidth()), 150));
	}
	Layout();
}
