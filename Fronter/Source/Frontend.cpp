#include "Frontend.h"
#include "Frames/MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(Frontend);

bool Frontend::OnInit()
{
	configuration = std::make_shared<Configuration::Configuration>();
	MainFrame* frame = new MainFrame("Paradox Game Converters: " + configuration->getSourceGame() + " To " + configuration->getTargetGame(), wxPoint(50, 50), wxSize(1200, 600));
	frame->loadConfiguration(configuration);
	frame->initFrame();
   frame->SetIcon(wxIcon(wxT("converter.ico"), wxBITMAP_TYPE_ICO, 16, 16));
   frame->Show(true);
   return true;
}
