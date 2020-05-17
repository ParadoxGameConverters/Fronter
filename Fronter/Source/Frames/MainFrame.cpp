#include "MainFrame.h"
#include "LogWindow.h"
#include "Tabs/ConvertTab.h"
#include "Tabs/OptionsTab.h"
#include "Tabs/PathsTab.h"
#include "wx/splitter.h"
#define tr localization->translate


MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame(NULL, wxID_ANY, title, pos, size)
{
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnSupportUs, this, wxID_NETWORK);
}

void MainFrame::initFrame()
{
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 400));
	notebook->SetMaxSize(wxSize(-1, 400));

	PathsTab* pathsTab = new PathsTab(notebook);
	pathsTab->loadConfiguration(configuration);
	pathsTab->loadLocalization(localization);
	pathsTab->initializePaths();
	pathsTab->SetBackgroundColour(wxColour(255, 245, 245));

	optionsTab = new OptionsTab(notebook);
	optionsTab->loadConfiguration(configuration);
	optionsTab->loadLocalization(localization);
	optionsTab->initializeOptions();
	optionsTab->SetBackgroundColour(wxColour(245, 255, 245));

	ConvertTab* convertTab = new ConvertTab(notebook);
	convertTab->loadConfiguration(configuration);
	convertTab->loadLocalization(localization);
	convertTab->loadSelf(this);
	convertTab->initializeConvert();
	convertTab->SetBackgroundColour(wxColour(245, 245, 255));

	notebook->AddPage(pathsTab, tr("PATHSTAB"));
	notebook->AddPage(optionsTab, tr("OPTIONSTAB"));
	notebook->AddPage(convertTab, tr("CONVERTTAB"));
	notebook->Layout();

	logWindow = new LogWindow(this, localization);
	logWindow->SetMinSize(wxSize(-1, 200));

	vbox->Add(notebook, wxSizerFlags(1).Expand().Border(wxALL, 1));
	vbox->Add(logWindow, wxSizerFlags(1).Expand().Border(wxALL, 1));

	this->SetSizer(vbox);
	this->Centre();
	Bind(wxEVT_SIZE, &MainFrame::onResize, this);
}

void MainFrame::onResize(wxSizeEvent& event)
{
	// layout everything in the dialog
	optionsTab->SetVirtualSize(event.GetSize());
	optionsTab->Layout();
	event.Skip();
}

void MainFrame::initSecondTail(const std::string& tailSource) const
{
	logWindow->initializeSecondTail(tailSource);
}

void MainFrame::terminateSecondTail() const
{
	logWindow->terminateSecondTail();
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(tr("ABOUTBODY"), tr("ABOUTTITLE"), wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSupportUs(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser("https://www.patreon.com/ParadoxGameConverters");
}

void MainFrame::OnLanguageChange(wxCommandEvent& event)
{
	localization->saveLanguage(event.GetId());
	wxMessageBox(tr("LANGUAGESAVEDBODY"), tr("LANGUAGESAVED"), wxOK | wxICON_INFORMATION);
}
