#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../Configuration/Configuration.h"
#include "../Utils/Localization/Localization.h"
#include "LogWindow.h"
#include "Tabs/OptionsTab.h"
#include <wx/notebook.h>

class MainFrame: public wxFrame
{
  public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void loadConfiguration(std::shared_ptr<Configuration> theConfiguration) { configuration = std::move(theConfiguration); }
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }
	void initFrame();
	void initSecondTail(const std::string& tailSource) const;
	void terminateSecondTail() const;

	void OnLanguageChange(wxCommandEvent& event);

  private:
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSupportUs(wxCommandEvent& event);
	LogWindow* logWindow = nullptr;
	wxNotebook* notebook = nullptr;
	OptionsTab* optionsTab = nullptr;

	void onResize(wxSizeEvent& evt);
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;
};
