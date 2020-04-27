#include "OptionBox.h"
#include "../../ConfigurationParser/Configuration.h"
#include "../../ConfigurationParser/Options/Option.h"
#include "Log.h"
#include <codecvt>
#include <wx/filepicker.h>
#include <wx/textctrl.h>

OptionBox::OptionBox(wxWindow* parent, const std::string& theName, std::shared_ptr<Configuration::Option> theOption):
	 wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxEXPAND), optionName(theName)
{
	option = theOption;
}

void OptionBox::initializeOption()
{
	SetMinSize(wxSize(280, 250));

	wxBoxSizer* boxSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(boxSizer);
	SetBackgroundColour(wxColour(240, 240, 240));

	wxWindow* boxHolder = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE | wxEXPAND);
	boxHolder->SetMinSize(wxSize(270, 240));

	boxSizer->Add(boxHolder, wxSizerFlags(1).Border(wxALL, 5));
	wxFlexGridSizer* flexGridSizer = new wxFlexGridSizer(1, 3, 3);
	boxHolder->SetSizer(flexGridSizer);
	boxHolder->SetBackgroundColour(wxColour(230, 230, 230));

	wxStaticText* st = new wxStaticText(boxHolder, wxID_ANY, option->getDisplayName(), wxDefaultPosition, wxDefaultSize);
	st->SetToolTip(option->getTooltip());
	flexGridSizer->Add(st, wxSizerFlags(1).Border(wxALL, 5));
	st->SetMinSize(wxSize(260, -1));
	st->Wrap(260);

	if (option->getRadioSelector().first)
	{
		auto first = true;
		for (const auto& radioOption: option->getRadioSelector().second->getOptions())
		{
			wxRadioButton* theButton;
			if (first)
			{
				theButton = new wxRadioButton(boxHolder,
					 radioOption->getID(),
					 radioOption->getDisplayName(),
					 wxDefaultPosition,
					 wxDefaultSize,
					 wxRB_GROUP | wxEXPAND);
				first = false;
			}
			else
			{
				theButton =
					 new wxRadioButton(boxHolder, radioOption->getID(), radioOption->getDisplayName(), wxDefaultPosition, wxDefaultSize, wxEXPAND);
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

			flexGridSizer->Add(theButton, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 5).Expand());
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
		textField = new wxTextCtrl(boxHolder, wxID_ANY, selector->getValue(), wxDefaultPosition, wxDefaultSize, flag);
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
		flexGridSizer->Add(textField, wxSizerFlags(1).Border(wxALL, 5).Expand().CenterHorizontal());
		SetMinSize(wxSize(std::max(textField->GetSize().GetWidth(), GetMinWidth()), 150));
	}

	boxHolder->Layout();
}
