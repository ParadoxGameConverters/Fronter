#include "Frontend.h"
#include "Frames/MainFrame.h"
#include "UpdateChecker/UpdateChecker.h"

wxIMPLEMENT_APP(Frontend);
#define tr localization->translate

bool Frontend::OnInit()
{
	localization = std::make_shared<Localization>();
	configuration = std::make_shared<Configuration>();
	auto* frame = new MainFrame(tr("TITLETITLE") + tr(configuration->getSourceGame()) + tr("TITLETO") + tr(configuration->getTargetGame()), wxPoint(50, 50), wxSize(1200, 600));
	frame->loadConfiguration(configuration);
	frame->loadLocalization(localization);
	frame->initFrame();
	frame->SetIcon(wxIcon(wxT("converter.ico"), wxBITMAP_TYPE_ICO, 16, 16));

	auto* menuFile = new wxMenu;
	if (configuration->getEnableUpdateChecker())
		menuFile->Append(wxID_REFRESH, tr("CHECKFORUPDATES"));
	menuFile->Append(wxID_EXIT, tr("EXIT"));
	auto* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, tr("ABOUT"));
	menuHelp->Append(wxID_NETWORK, tr("SUPPORT_US"));
	auto* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&" + tr("MENUCONVERTER"));
	auto* menuLanguages = new wxMenu;

	auto i = 0;
	for (const auto& language: localization->getLoadedLanguages())
	{
		auto* menuLanguage = new wxMenuItem(menuLanguages, i, localization->translateLanguage(language), localization->translateLanguage(language));
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


	// check for updates on startup
	if (configuration->getEnableUpdateChecker() &&
		configuration->getCheckForUpdatesOnStartup() &&
		isUpdateAvailable("commit_id.txt", configuration->getPagesCommitIdUrl()))
	{
		const auto info = getLatestReleaseInfo(configuration->getName());
		const auto msgBody = getUpdateMessageBody(tr("NEWVERSIONBODY"), info);
		if (wxMessageBox(msgBody, tr("NEWVERSIONTITLE"), wxYES_NO | wxICON_INFORMATION) == wxYES)
		{
			if (info.zipURL)
			{
				startUpdaterAndDie(*info.zipURL, configuration->getConverterFolder());
			}
			else
			{
				wxLaunchDefaultBrowser(configuration->getConverterReleaseForumThread());
				wxLaunchDefaultBrowser(configuration->getLatestGitHubConverterReleaseUrl());
			}
		}
	}
	
	return true;
}
