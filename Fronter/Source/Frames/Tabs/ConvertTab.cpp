#include "ConvertTab.h"
#include "../MainFrame.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <wx/wrapsizer.h>
#define tr localization->translate

ConvertTab::ConvertTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	Bind(wxEVT_CONVERTERDONE, &ConvertTab::onConverterDone, this, wxID_ANY);
	Bind(wxEVT_COPIERDONE, &ConvertTab::onCopierDone, this, wxID_ANY);
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

	wxStaticText* cell11 = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSAVING"));
	statusSave = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSTATUSPRE"));
	wxStaticText* cell21 = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTCONVERTING"));
	statusConvert = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSTATUSPRE"));
	wxStaticText* cell31 = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTCOPYING"));
	statusCopy = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSTATUSPRE"));

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
	statusSave->SetLabel(tr("CONVERTSTATUSPRE"));
	statusConvert->SetLabel(tr("CONVERTSTATUSPRE"));
	statusCopy->SetLabel(tr("CONVERTSTATUSPRE"));

	statusSave->SetLabel(tr("CONVERTSTATUSIN"));
	if (configuration->exportConfiguration())
		statusSave->SetLabel(tr("CONVERTSTATUSPOSTSUCCESS"));
	else
	{
		statusSave->SetLabel(tr("CONVERTSTATUSPOSTFAIL"));
		return;
	}

	statusConvert->SetLabel(tr("CONVERTSTATUSIN"));
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
		statusConvert->SetLabel(tr("CONVERTSTATUSPOSTSUCCESS"));
		wxMilliSleep(400); // waiting on second tail to finish transcribing.
		mainFrame->terminateSecondTail();
	}
	else
	{
		statusConvert->SetLabel(tr("CONVERTSTATUSPOSTFAIL"));
		mainFrame->terminateSecondTail();
		convertButton->Enable();
		return;
	}
	statusCopy->SetLabel(tr("CONVERTSTATUSIN"));
	modCopier = new ModCopier(this);
	modCopier->loadConfiguration(configuration);
	modCopier->Create();
	modCopier->Run();
}

void ConvertTab::onCopierDone(wxCommandEvent& event)
{
	const auto message = event.GetInt();
	if (message)
		statusCopy->SetLabel(tr("CONVERTSTATUSPOSTSUCCESS"));
	else
	{
		statusCopy->SetLabel(tr("CONVERTSTATUSPOSTFAIL"));
	}
	convertButton->Enable();
}