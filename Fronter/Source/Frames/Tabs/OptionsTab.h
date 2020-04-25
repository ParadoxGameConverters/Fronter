#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../ConfigurationParser/Configuration.h"
#include <wx/filepicker.h>
#include <wx/notebook.h>

class OptionsTab: public wxNotebookPage
{
  public:
	OptionsTab(wxWindow* parent);

	[[nodiscard]] const auto& getTabName() const { return tabName; }

	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializeOptions();

  private:
	std::string tabName = "Options";
	wxDECLARE_EVENT_TABLE();
	std::shared_ptr<Configuration::Configuration> configuration;
};
