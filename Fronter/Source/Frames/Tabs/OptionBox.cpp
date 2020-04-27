#include "OptionBox.h"
#include "../../ConfigurationParser/Options/Option.h"
#include "Log.h"
#include <wx/filepicker.h>
#include <wx/textctrl.h>
#include "../../ConfigurationParser/Configuration.h"
#include <codecvt>

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
			if (!option->getRadioSelector().second->getSelectedValue().empty() &&
				 std::stoi(option->getRadioSelector().second->getSelectedValue()) == radioOption->getID())
			{
				theButton->SetValue(true);
			}
			else
			{
				theButton->SetValue(radioOption->isDefault());
			}			

			theButton->Bind(wxEVT_RADIOBUTTON, [this](wxCommandEvent& event) {
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
		textField = new wxTextCtrl(this, wxID_ANY, selector->getValue(), wxDefaultPosition, wxDefaultSize, flag);
		textField->SetToolTip(selector->getTooltip());		

		textField->Bind(wxEVT_TEXT, [this](wxCommandEvent& event) {
			std::wstring theString = event.GetString().ToStdWstring();
			std::u16string u16str(theString.begin(), theString.end());
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
			std::string result = conversion.to_bytes(u16str);
			result = Configuration::Configuration::normalizeStringPath(result);
			option->setTextSelectorValue(result);
			if (event.GetString() != wxString(result))
			{
				textField->SetValue(result);
			}			
		});
		flexGridSizer->Add(textField);
		SetMinSize(wxSize(std::max(textField->GetSize().GetWidth(), GetMinWidth()), 150));
	}
	Layout();
}
