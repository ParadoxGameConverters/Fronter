#include "ModsTab.h"
#include "../MainFrame.h"
#include <wx/wrapsizer.h>
#define tr localization->translate

ModsTab::ModsTab(wxWindow* parent): wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	m_pParent = parent;
	modsTabSizer = new wxBoxSizer(wxVERTICAL);
	SetScrollRate(0, 16);
	SetSizer(modsTabSizer);
	title = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	modsTabSizer->Add(title, wxSizerFlags(0).Border(wxLEFT | wxRIGHT | wxTOP, 5));
}

void ModsTab::initializeMods()
{
	if (configuration->getAutoGenerateModsFrom().empty())
	{
		title->SetLabel(tr("MODSDISABLED"));
		return;
	}

	if (configuration->getAutoLocatedMods().empty())
	{
		title->SetLabel(tr("MODSNOTFOUND"));
		return;
	}

	title->SetLabel(tr("MODSFOUND"));

	// This is the mod box.
	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	wxWindow* boxHolder = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE | wxEXPAND);

	modsTabSizer->Add(boxHolder, wxSizerFlags(1).Border(wxALL, 5));
	boxHolder->SetSizer(boxSizer);
	boxHolder->SetBackgroundColour(wxColour(230, 230, 230));

	auto modCounter = 0;
	for (const auto& mod: configuration->getAutoLocatedMods())
	{
		wxCheckBox* theCheckBox =
			 new wxCheckBox(boxHolder, modCounter, mod.getName(), wxDefaultPosition, wxDefaultSize, wxEXPAND, wxDefaultValidator, mod.getFileName());
		theCheckBox->SetToolTip(mod.getFileName());

		if (configuration->getPreloadedModFileNames().count(mod.getFileName()))
		{
			theCheckBox->SetValue(true);
		}
		else
		{
			theCheckBox->SetValue(false);
		}

		theCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event) {
			std::set<int> toSelect;
			for (const auto& checkPtr: checkBoxes)
			{
				if (checkPtr->GetValue())
					toSelect.insert(checkPtr->GetId());
			}
			configuration->setEnabledMods(toSelect);
		});

		checkBoxes.emplace_back(theCheckBox);
		boxSizer->Add(theCheckBox, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 5).Expand());
		++modCounter;
	}

	GetParent()->Layout();
}
