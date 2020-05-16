#include "Frontend.h"
#include "Frames/MainFrame.h"

wxIMPLEMENT_APP(Frontend);
#define tr localization->translate

bool Frontend::OnInit()
{
	localization = std::make_shared<Localization>();
	configuration = std::make_shared<Configuration>();
	
	MainFrame* frame =
		 new MainFrame(tr("TITLETITLE") + tr(configuration->getSourceGame()) + tr("TITLETO") + tr(configuration->getTargetGame()), wxPoint(50, 50), wxSize(1200, 600));
	frame->loadConfiguration(configuration);
	frame->loadLocalization(localization);
	frame->initFrame();
	frame->SetIcon(wxIcon(wxT("converter.ico"), wxBITMAP_TYPE_ICO, 16, 16));

	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT, tr("EXIT"));
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, tr("ABOUT"));
	menuHelp->Append(wxID_NETWORK, tr("SUPPORT_US"));
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&" + tr("MENUCONVERTER"));
	wxMenu* menuLanguages = new wxMenu;

	auto i = 0;
	for (const auto& language: localization->getLoadedLanguages())
	{
		wxMenuItem* menuLanguage = new wxMenuItem(menuLanguages, i, localization->translateLanguage(language), localization->translateLanguage(language));
		frame->Bind(wxEVT_MENU, &MainFrame::OnLanguageChange, frame, i);
		menuLanguages->Append(menuLanguage);
		++i;
	}

	menuBar->Append(menuLanguages, "&" + tr("LANGUAGE"));
	menuBar->Append(menuHelp, "&" + tr("MENUPGCG"));

	frame->SetMenuBar(menuBar);
	frame->CreateStatusBar();
	frame->SetStatusText(tr("FOOTER"));

	frame->Show(true);
	return true;
}
