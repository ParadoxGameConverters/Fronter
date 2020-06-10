#include "OptionBox.h"
#include "../../Configuration/Configuration.h"
#include "../../Configuration/Options/Option.h"
#include "OSCompatibilityLayer.h"
#include <wx/textctrl.h>
#define tr localization->translate

OptionBox::OptionBox(wxWindow* parent, const std::string& theName, std::shared_ptr<Option> theOption):
	 wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxEXPAND), optionName(theName)
{
	option = theOption;
}

void OptionBox::initializeOption()
{
	Log(LogLevel::Debug) << "start init";
	SetMinSize(wxSize(280, 250));

	wxBoxSizer* boxSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(boxSizer);
	SetBackgroundColour(wxColour(240, 240, 240));
	Log(LogLevel::Debug) << "1";

	wxWindow* boxHolder = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE | wxEXPAND);
	boxHolder->SetMinSize(wxSize(270, 240));
	Log(LogLevel::Debug) << "2";

	boxSizer->Add(boxHolder, wxSizerFlags(1).Border(wxALL, 5));
	wxFlexGridSizer* flexGridSizer = new wxFlexGridSizer(1, 3, 3);
	Log(LogLevel::Debug) << "3";
	boxHolder->SetSizer(flexGridSizer);
	boxHolder->SetBackgroundColour(wxColour(230, 230, 230));
	Log(LogLevel::Debug) << "4";

	wxStaticText* st;
	if (localizationOverride)
		st = new wxStaticText(boxHolder, wxID_ANY, option->getDisplayName(), wxDefaultPosition, wxDefaultSize);
	else
		st = new wxStaticText(boxHolder, wxID_ANY, tr(option->getDisplayName()), wxDefaultPosition, wxDefaultSize);
	Log(LogLevel::Debug) << "4a";
	st->SetToolTip(tr(option->getTooltip()));
	Log(LogLevel::Debug) << "5";
	flexGridSizer->Add(st, wxSizerFlags(1).Border(wxALL, 5));
	st->SetMinSize(wxSize(260, -1));
	st->Wrap(260);

	Log(LogLevel::Debug) << "start crap";
	
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
			const auto result = Utils::UTF16ToUTF8(event.GetString().ToStdWstring());
			option->setTextSelectorValue(result);
			if (event.GetString() != wxString(result))
			{
				textField->SetValue(result);
			}
		});
		flexGridSizer->Add(textField, wxSizerFlags(1).Border(wxALL, 5).Expand().CenterHorizontal());
		SetMinSize(wxSize(std::max(textField->GetSize().GetWidth(), GetMinWidth()), 150));
	}

	Log(LogLevel::Debug) << "To checkbox";

	if (option->getCheckBoxSelector().first)
	{
		for (const auto& checkBoxOption: option->getCheckBoxSelector().second->getOptions())
		{
			Log(LogLevel::Debug) << "In checkboxoption " << checkBoxOption->getName() << " with ID " << checkBoxOption->getID();
			wxCheckBox* theCheckBox;
			if (localizationOverride)
			{
				theCheckBox = new wxCheckBox(boxHolder,
					 checkBoxOption->getID(),
					 checkBoxOption->getDisplayName(),
					 wxDefaultPosition,
					 wxDefaultSize,
					 wxEXPAND,
					 wxDefaultValidator,
					 checkBoxOption->getName());
				theCheckBox->SetToolTip(checkBoxOption->getTooltip());
			}
			else
			{
				theCheckBox = new wxCheckBox(boxHolder,
					 checkBoxOption->getID(),
					 tr(checkBoxOption->getDisplayName()),
					 wxDefaultPosition,
					 wxDefaultSize,
					 wxEXPAND,
					 wxDefaultValidator,
					 checkBoxOption->getName());
				theCheckBox->SetToolTip(tr(checkBoxOption->getTooltip()));
			}

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

			Log(LogLevel::Debug) << "on bind";
			
			theCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event) {
				std::set<int> toSelect;
				for (const auto& checkPtr: checkBoxes)
				{
					if (checkPtr->GetValue())
						toSelect.insert(checkPtr->GetId());
				}
				option->setCheckBoxSelectorIDs(toSelect);
			});

			Log(LogLevel::Debug) << "to sizer";
			
			checkBoxes.emplace_back(theCheckBox);
			flexGridSizer->Add(theCheckBox, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 5).Expand());
			SetMinSize(wxSize(std::max(theCheckBox->GetSize().GetWidth(), GetMinWidth()), 150));
		}
	}

	boxHolder->Layout();
}
