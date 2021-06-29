#include "ConvertTab.h"
#include "../MainFrame.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <wx/wrapsizer.h>
#include <ranges>

#define tr localization->translate

wxDEFINE_EVENT(wxEVT_LOGLEVELCHANGED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_BLANKLOG, wxCommandEvent);

ConvertTab::ConvertTab(wxWindow* parent): wxNotebookPage(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	m_pParent = parent;
	Bind(wxEVT_CONVERTERDONE, &ConvertTab::onConverterDone, this);
	Bind(wxEVT_COPIERDONE, &ConvertTab::onCopierDone, this);
}

void ConvertTab::initializeConvert()
{
	// Initialize a flex which will have 3 col, 4 rows
	auto* convertSizer = new wxGridSizer(4, 3, 0, 0);
	SetSizer(convertSizer);
	// convertSizer->SetVGap(50);

	auto* logPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	logPanel->SetBackgroundColour(wxColour(230, 230, 245));
	auto* logSizer = new wxGridSizer(1, 1, 5, 5);
	logPanel->SetSizer(logSizer);

	wxArrayString boxChoices;
	boxChoices.Add(tr("LOGLEVEL0"));
	boxChoices.Add(tr("LOGLEVEL1"));
	boxChoices.Add(tr("LOGLEVEL2"));
	boxChoices.Add(tr("LOGLEVEL3"));
	
	auto* theButtonBox = new wxRadioBox(logPanel, wxID_ANY, tr("LOGLEVEL"), wxDefaultPosition, wxDefaultSize, boxChoices, 4);
	theButtonBox->SetSelection(1);
	theButtonBox->Bind(wxEVT_RADIOBOX, [this](wxCommandEvent& event) {
		wxCommandEvent evt(wxEVT_LOGLEVELCHANGED);
		evt.SetInt(event.GetInt());
		m_pParent->AddPendingEvent(evt);
	});

	logSizer->Add(theButtonBox, wxSizerFlags(0).Center());

	convertSizer->Add(logPanel, wxSizerFlags(0).Center());

	// In the first cell goes a 2cx3r status table
	auto* statusPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	statusPanel->SetBackgroundColour(wxColour(240, 240, 240));
	auto* statusSizer = new wxGridSizer(3, 2, 5, 5);
	statusPanel->SetSizer(statusSizer);

	auto* cell11 = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSAVING"));
	statusSave = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSTATUSPRE"));
	auto* cell21 = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTCONVERTING"));
	statusConvert = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSTATUSPRE"));
	auto* cell31 = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTCOPYING"));
	statusCopy = new wxStaticText(statusPanel, wxID_ANY, tr("CONVERTSTATUSPRE"));

	statusSizer->Add(cell11);
	statusSizer->Add(statusSave, wxSizerFlags(1).Align(1).CenterHorizontal());
	statusSizer->Add(cell21);
	statusSizer->Add(statusConvert, wxSizerFlags(1).Align(1).CenterHorizontal());
	statusSizer->Add(cell31);
	statusSizer->Add(statusCopy, wxSizerFlags(1).Align(1).CenterHorizontal());

	convertSizer->Add(statusPanel, wxSizerFlags(0).Center());
	convertSizer->AddStretchSpacer(0);

	// second row the gauge

	// In goes a 1cx2r holder table
	auto* gaugePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	gaugePanel->SetBackgroundColour(wxColour(230, 245, 230));
	auto* gaugeSizer = new wxGridSizer(2, 1, 5, 5);
	gaugePanel->SetSizer(gaugeSizer);

	// Scale goes to 109.
	gauge = new wxGauge(gaugePanel, wxID_ANY, 109, wxDefaultPosition, wxDefaultSize, wxHORIZONTAL);
	gaugeCounter = new wxStaticText(gaugePanel, wxID_ANY, "0%");

	gaugeSizer->Add(gauge, wxSizerFlags(1).Top().CenterHorizontal());
	gaugeSizer->Add(gaugeCounter, wxSizerFlags(1).Top().CenterHorizontal());

	convertSizer->AddStretchSpacer(0);
	convertSizer->Add(gaugePanel, wxSizerFlags(1).Top().CenterHorizontal());
	convertSizer->AddStretchSpacer(0);

	// in third row goes a button

	convertButton = new wxButton(this, wxID_ANY, tr("CONVERTBUTTON"), wxDefaultPosition, wxDefaultSize);
	convertButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxEventFunction)&ConvertTab::onConvertStarted, nullptr, this);

	convertSizer->AddStretchSpacer(0);
	convertSizer->Add(convertButton, wxSizerFlags(1).Top().CenterHorizontal());
	convertSizer->AddStretchSpacer(0);

	// Fourth row is blank for cosmetics.
	
	Layout();
}

void ConvertTab::onConvertStarted(wxCommandEvent& WXUNUSED(event))
{
	configuration->clearSecondLog();
	wxCommandEvent evt(wxEVT_BLANKLOG);
	m_pParent->AddPendingEvent(evt);

	for (const auto& folder: configuration->getRequiredFolders() | std::views::values)
	{
		if (folder->isMandatory() && !commonItems::DoesFolderExist(folder->getValue()))
		{
			Log(LogLevel::Error) << "Launching converter failed - mandatory folder " << folder->getName() << " at " << folder->getValue() << " not found.";
			return;
		}
	}
	for (const auto& file: configuration->getRequiredFiles() | std::views::values)
	{
		if (file->isMandatory() && !commonItems::DoesFileExist(file->getValue()))
		{
			Log(LogLevel::Error) << "Launching converter failed - mandatory file " << file->getName() << " at " << file->getValue() << " not found.";
			return;
		}
	}
	
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
	wxMilliSleep(400); // waiting on second tail to finish transcribing.
	const auto message = event.GetInt();
	mainFrame->terminateSecondTail();
	if (message)
	{
		statusConvert->SetLabel(tr("CONVERTSTATUSPOSTSUCCESS"));
	}
	else
	{
		statusConvert->SetLabel(tr("CONVERTSTATUSPOSTFAIL"));
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
	{
		statusCopy->SetLabel(tr("CONVERTSTATUSPOSTSUCCESS"));
		setProgress(109);
	}	
	else
	{
		statusCopy->SetLabel(tr("CONVERTSTATUSPOSTFAIL"));
	}
	convertButton->Enable();
}

void ConvertTab::setProgress(const int progress)
{
	gauge->SetValue(progress);
	gaugeCounter->SetLabel(std::to_string(progress) + "%");
}
