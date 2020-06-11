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
	modsTabSizer->Add(title);
}

void ModsTab::initializeMods()
{
	if (configuration->getAutoGenerateModsFrom().empty())
	{
		title->SetLabel("not enabled");
		return;
	}

	if (configuration->getAutoLocatedMods().empty())
	{
		title->SetLabel("no mods found");
		return;
	}

	title->SetLabel(tr("MODSTAB"));

	// This is the mod box.
	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxHolder = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE | wxEXPAND);

	modsTabSizer->Add(boxHolder, wxSizerFlags(1).Border(wxALL, 5));
	boxHolder->SetSizer(boxSizer);
	boxHolder->SetBackgroundColour(wxColour(230, 230, 230));

	auto modCounter = 0;
	for (const auto& modName: configuration->getAutoLocatedMods())
	{
		wxCheckBox* theCheckBox = new wxCheckBox(boxHolder,
			 modCounter,
			 modName,
			 wxDefaultPosition,
			 wxDefaultSize,
			 wxEXPAND,
			 wxDefaultValidator,
			 modName);
		theCheckBox->SetToolTip(modName);

		checkBoxes.emplace_back(theCheckBox);
		boxSizer->Add(theCheckBox, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 5).Expand());
		++modCounter;
	}

	GetParent()->Layout();
}
