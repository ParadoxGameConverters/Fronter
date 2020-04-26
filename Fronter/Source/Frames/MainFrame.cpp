#include "MainFrame.h"
#include "LogWindow.h"
#include "Tabs/PathsTab.h"
#include "wx/splitter.h"
#include <wx/notebook.h>
#include "Tabs/OptionsTab.h"
#include "Tabs/ConvertTab.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Paradox Game Converters Group");
}

void MainFrame::initFrame()
{
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

	wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 400));
	notebook->SetMaxSize(wxSize(-1, 400));

	PathsTab* pathsTab = new PathsTab(notebook);
	pathsTab->loadConfiguration(configuration);
	pathsTab->initializePaths();

	OptionsTab* optionsTab = new OptionsTab(notebook);
	optionsTab->loadConfiguration(configuration);
	optionsTab->initializeOptions();

	ConvertTab* convertTab = new ConvertTab(notebook);
	convertTab->loadConfiguration(configuration);
	convertTab->loadSelf(this);
	convertTab->initializeConvert();

	notebook->AddPage(convertTab, convertTab->getTabName());
	notebook->AddPage(pathsTab, pathsTab->getTabName());
	notebook->AddPage(optionsTab, optionsTab->getTabName());
	notebook->Layout();

	logWindow = new LogWindow(this);
	logWindow->SetMinSize(wxSize(-1, 200));

	vbox->Add(notebook, wxSizerFlags(1).Expand().Border(wxALL, 1));
	vbox->Add(logWindow, wxSizerFlags(1).Expand().Border(wxALL, 1));

	this->SetSizer(vbox);
	this->Centre();
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
	std::string message = "Copyright (c) 2020 The Paradox Game Converters Group\n";
	message += "\n";
	message += "This converter, as all others, is free and available at our Github repository.\n ";
	message += "\n";
	message += "Github. Forums. Wiki pages. Steam. If you need to find us, report bugs, offer suggestions or wish to participate, we're there.";

	wxMessageBox(message, "Paradox Game Converters Group - Universal Frontend", wxOK | wxICON_INFORMATION);
}
