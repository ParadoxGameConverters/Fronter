#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../ConfigurationParser/Configuration.h"

class MainFrame: public wxFrame
{
  public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = std::move(theConfiguration); }
	void initFrame();

  private:
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();

	std::shared_ptr<Configuration::Configuration> configuration;
};