#include "MainFrame.h"
#include "LogWindow.h"
#include "wx/splitter.h"
#include <wx/notebook.h>
#include "Tabs/PathsTab.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Paradox Game Converters Team");
}

void MainFrame::initFrame()
{
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
		
	wxPanel* notePanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(1200, 400));	
	wxNotebook* notebook = new wxNotebook(notePanel, wxID_ANY, wxPoint(0, 0), wxSize(1200, 400));

	PathsTab* pathsTab = new PathsTab(notebook);
	pathsTab->loadConfiguration(configuration);
	pathsTab->initializePaths();
	
	notebook->AddPage(pathsTab, pathsTab->getTabName());
	notebook->AddPage(new wxNotebookPage(notebook, -1), L"Options");
	notebook->AddPage(new wxNotebookPage(notebook, -1), L"Convert");
	notebook->Layout();
		
	wxPanel* logPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 400), wxSize(1200, 200));
	LogWindow* logWindow = new LogWindow(logPanel, wxPoint(0, 400), wxSize(1200, 200));
	logWindow->SetScrollbar(wxVERTICAL, 0, 16, 50);

	vbox->Add(notePanel, 1, wxEXPAND | wxALL, 1);
	vbox->Add(logPanel, 1, wxEXPAND | wxALL, 1);

	this->SetSizer(vbox);
	this->Centre();
	
}

void MainFrame::OnExit(wxCommandEvent& event)
{
   Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	std::string message = "Copyright (c) 2014 The Paradox Game Converters Group\n";
	message += "\n";
	message += "This converter, as all others, is free and available at our Github repository.\n ";
	message += "\n";
	message += "Github. Forums. Wiki pages. Steam. If you need to find us, report bugs, offer suggestions or wish to participate, we're there.";
					
   wxMessageBox(message, "Paradox Game Converters Group - Universal Frontend", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event)
{
   wxLogMessage("Hello world from wxWidgets!");
}
