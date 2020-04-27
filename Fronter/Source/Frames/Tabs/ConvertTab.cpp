#include "ConvertTab.h"
#include "../MainFrame.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <wx/wrapsizer.h>

BEGIN_EVENT_TABLE(ConvertTab, wxNotebookPage)
EVT_COMMAND(wxID_ANY, wxEVT_CONVERTERDONE, ConvertTab::onConverterDone)
END_EVENT_TABLE()

ConvertTab::ConvertTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
}

void ConvertTab::initializeConvert()
{
	// Initialize a flex which will have 3 col, 2 rows
	wxGridSizer* convertSizer = new wxGridSizer(3, 3, 0, 0);
	SetSizer(convertSizer);
	// convertSizer->SetVGap(50);

	// In the first cell goes a 2cx3r status table
	wxPanel* statusPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	statusPanel->SetBackgroundColour(wxColour(240, 240, 240));
	wxGridSizer* statusSizer = new wxGridSizer(3, 2, 5, 5);
	statusPanel->SetSizer(statusSizer);

	wxStaticText* cell11 = new wxStaticText(statusPanel, wxID_ANY, "Saving Configuration:");
	statusSave = new wxStaticText(statusPanel, wxID_ANY, "Not Started.");
	wxStaticText* cell21 = new wxStaticText(statusPanel, wxID_ANY, "Converting Save:");
	statusConvert = new wxStaticText(statusPanel, wxID_ANY, "Not Started.");
	wxStaticText* cell31 = new wxStaticText(statusPanel, wxID_ANY, "Copying Mod:");
	statusCopy = new wxStaticText(statusPanel, wxID_ANY, "Not Started.");

	statusSizer->Add(cell11);
	statusSizer->Add(statusSave, wxSizerFlags(1).Align(1).CenterHorizontal());
	statusSizer->Add(cell21);
	statusSizer->Add(statusConvert, wxSizerFlags(1).Align(1).CenterHorizontal());
	statusSizer->Add(cell31);
	statusSizer->Add(statusCopy, wxSizerFlags(1).Align(1).CenterHorizontal());

	convertSizer->AddStretchSpacer(0);
	convertSizer->Add(statusPanel, wxSizerFlags(0).Center());
	convertSizer->AddStretchSpacer(0);

	// in second row goes a button

	convertButton = new wxButton(this, wxID_ANY, "Convert Save", wxDefaultPosition, wxDefaultSize);
	convertButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxEventFunction)&ConvertTab::onConvertStarted, nullptr, this);

	convertSizer->AddStretchSpacer(0);
	convertSizer->Add(convertButton, wxSizerFlags(1).Top().CenterHorizontal());
	convertSizer->AddStretchSpacer(0);

	Layout();
}

void ConvertTab::onConvertStarted(wxCommandEvent& event)
{
	convertButton->Disable();
	// Reset statuses
	statusSave->SetLabel("Not Started.");
	statusConvert->SetLabel("Not Started.");
	statusCopy->SetLabel("Not Started.");

	statusSave->SetLabel("In Progress.");
	if (configuration->exportConfiguration())
		statusSave->SetLabel("Finished.");
	else
	{
		statusSave->SetLabel("Failed!");
		return;
	}

	statusConvert->SetLabel("In Progress.");
	converterLauncher = new ConverterLauncher(this);
	converterLauncher->loadConfiguration(configuration);
	converterLauncher->Create();
	converterLauncher->Run();
	mainFrame->initSecondTail(configuration->getSecondTailSource());
}

void ConvertTab::onConverterDone(wxCommandEvent& event)
{
	const auto message = event.GetInt();
	if (message)
	{
		statusConvert->SetLabel("Finished.");
		wxMilliSleep(400); // waiting on second tail to finish transcribing.
		mainFrame->terminateSecondTail();
	}
	else
	{
		statusConvert->SetLabel("Failed!");
		mainFrame->terminateSecondTail();
		convertButton->Enable();
		return;
	}
	statusCopy->SetLabel("Copying Mod.");
	if (configuration->copyMod())
		statusCopy->SetLabel("Finished.");
	else
	{
		statusCopy->SetLabel("Failed!");
	}
	convertButton->Enable();
}
