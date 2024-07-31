#include "OptionBox.h"
#include "../../Configuration/Configuration.h"
#include "../../Configuration/Options/Option.h"
#include "OSCompatibilityLayer.h"
#include <wx/textctrl.h>
#define tr localization->translate

OptionBox::OptionBox(wxWindow* parent, const std::string& theName, const std::shared_ptr<Option>& theOption):
	 wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxEXPAND), optionName(theName)
{
	option = theOption;
}

void OptionBox::initializeOption()
{
	SetMinSize(wxSize(280, 250));

	auto* boxSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(boxSizer);
	SetBackgroundColour(wxColour(240, 240, 240));

	auto* boxHolder = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE | wxEXPAND);
	boxHolder->SetMinSize(wxSize(270, 240));

	boxSizer->Add(boxHolder, wxSizerFlags(1).Border(wxALL, 5));
	auto* flexGridSizer = new wxFlexGridSizer(1, 3, 3);
	boxHolder->SetSizer(flexGridSizer);
	boxHolder->SetBackgroundColour(wxColour(230, 230, 230));

	auto* st = new wxStaticText(boxHolder, wxID_ANY, tr(option->getDisplayName()), wxDefaultPosition, wxDefaultSize);
	st->SetToolTip(tr(option->getTooltip()));
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
				theButton =
					 new wxRadioButton(boxHolder, radioOption->getID(), tr(radioOption->getDisplayName()), wxDefaultPosition, wxDefaultSize, wxRB_GROUP | wxEXPAND);
				first = false;
			}
			else
			{
				theButton = new wxRadioButton(boxHolder, radioOption->getID(), tr(radioOption->getDisplayName()), wxDefaultPosition, wxDefaultSize, wxEXPAND);
			}
			theButton->SetToolTip(tr(radioOption->getTooltip()));
			if (!option->getRadioSelector().second->getSelectedValue().empty() && option->getRadioSelector().second->getSelectedID() == radioOption->getID())
			{
				theButton->SetValue(true);
			}
			else
			{
				theButton->SetValue(radioOption->isDefault());
			}

			theButton->Bind(wxEVT_RADIOBUTTON, [this](wxCommandEvent& event) {
				option->setRadioSelectorID(event.GetId());
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
		textField->SetToolTip(tr(selector->getTooltip()));

		textField->Bind(wxEVT_TEXT, [this](wxCommandEvent& event) {
			const auto result = commonItems::convertUTF8ToASCII(commonItems::UTF16ToUTF8(event.GetString().ToStdWstring()));
			option->setTextSelectorValue(result);
			if (event.GetString() != wxString(result))
			{
				textField->SetValue(result);
			}
		});
		flexGridSizer->Add(textField, wxSizerFlags(1).Border(wxALL, 5).Expand().CenterHorizontal());
		SetMinSize(wxSize(std::max(textField->GetSize().GetWidth(), GetMinWidth()), 150));
	}

	if (option->getCheckBoxSelector().first)
	{
		for (const auto& checkBoxOption: option->getCheckBoxSelector().second->getOptions())
		{
			auto* theCheckBox = new wxCheckBox(boxHolder,
				 checkBoxOption->getID(),
				 tr(checkBoxOption->getDisplayName()),
				 wxDefaultPosition,
				 wxDefaultSize,
				 wxEXPAND,
				 wxDefaultValidator,
				 checkBoxOption->getName());
			theCheckBox->SetToolTip(tr(checkBoxOption->getTooltip()));
			if (option->isCheckBoxSelectorPreloaded())
			{
				if (option->getCheckBoxSelector().second->getSelectedIDs().count(checkBoxOption->getID()))
				{
					theCheckBox->SetValue(true);
				}
				else
				{
					theCheckBox->SetValue(false);
				}
			}
			else
			{
				theCheckBox->SetValue(checkBoxOption->isDefault());
			}

			theCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& WXUNUSED(event)) {
				std::set<int> toSelect;
				for (const auto& checkPtr: checkBoxes)
				{
					if (checkPtr->GetValue())
						toSelect.insert(checkPtr->GetId());
				}
				option->setCheckBoxSelectorIDs(toSelect);
			});

			checkBoxes.emplace_back(theCheckBox);
			flexGridSizer->Add(theCheckBox, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 5).Expand());
			SetMinSize(wxSize(std::max(theCheckBox->GetSize().GetWidth(), GetMinWidth()), 150));
		}
	}

	boxHolder->Layout();
}
