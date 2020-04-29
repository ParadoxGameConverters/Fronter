#include "Frontend.h"
#include "Frames/MainFrame.h"

wxIMPLEMENT_APP(Frontend);

bool Frontend::OnInit()
{
	configuration = std::make_shared<Configuration>();
	MainFrame* frame = new MainFrame("Paradox Game Converters: " + configuration->getSourceGame() + " To " + configuration->getTargetGame(),
		 wxPoint(50, 50),
		 wxSize(1200, 600));
	frame->loadConfiguration(configuration);
	frame->initFrame();
	frame->SetIcon(wxIcon(wxT("converter.ico"), wxBITMAP_TYPE_ICO, 16, 16));

	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&Converter");
	menuBar->Append(menuHelp, "&PGCG");

	frame->SetMenuBar(menuBar);
	frame->CreateStatusBar();
	frame->SetStatusText("Paradox Game Converters Group");

	frame->Show(true);
	return true;
}
