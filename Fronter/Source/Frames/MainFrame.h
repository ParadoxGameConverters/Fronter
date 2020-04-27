#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../ConfigurationParser/Configuration.h"
#include "LogWindow.h"
#include <wx/notebook.h>
#include "Tabs/OptionsTab.h"

class MainFrame: public wxFrame
{
  public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = std::move(theConfiguration); }
	void initFrame();
	void initSecondTail(const std::string& tailSource) const;
	void terminateSecondTail() const;

  private:
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	LogWindow* logWindow = nullptr;
	wxNotebook* notebook = nullptr;
	OptionsTab* optionsTab = nullptr;
	
	wxDECLARE_EVENT_TABLE();
	void onResize(wxSizeEvent& evt);
	std::shared_ptr<Configuration::Configuration> configuration;
};
