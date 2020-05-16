#include "Frontend.h"
#include "Frames/MainFrame.h"

wxIMPLEMENT_APP(Frontend);
#define tr localization->translate

bool Frontend::OnInit()
{
    Log(LogLevel::Debug) << "init";
	localization = std::make_shared<Localization>();
    Log(LogLevel::Debug) << "loc";
	configuration = std::make_shared<Configuration>();
    Log(LogLevel::Debug) << "conf to frame";

	MainFrame* frame =
		 new MainFrame(tr("TITLETITLE") + tr(configuration->getSourceGame()) + tr("TITLETO") + tr(configuration->getTargetGame()), wxPoint(50, 50), wxSize(1200, 600));
    Log(LogLevel::Debug) << "1";
	frame->loadConfiguration(configuration);
    Log(LogLevel::Debug) << "1a";
	frame->loadLocalization(localization);
    Log(LogLevel::Debug) << "1b";
	frame->initFrame();
    Log(LogLevel::Debug) << "1c";
	frame->SetIcon(wxIcon(wxT("converter.ico"), wxBITMAP_TYPE_ICO, 16, 16));
    Log(LogLevel::Debug) << "2";

	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT, tr("EXIT"));
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, tr("ABOUT"));
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&" + tr("MENUCONVERTER"));
	wxMenu* menuLanguages = new wxMenu;
    Log(LogLevel::Debug) << "3";

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
    Log(LogLevel::Debug) << "3a";

	frame->SetMenuBar(menuBar);
	frame->CreateStatusBar();
	frame->SetStatusText(tr("FOOTER"));
    Log(LogLevel::Debug) << "4";

	frame->Show(true);
	return true;
}
