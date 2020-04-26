#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../ConfigurationParser/Configuration.h"
#include <wx/filepicker.h>
#include <wx/notebook.h>

class ConvertTab: public wxNotebookPage
{
  public:
	ConvertTab(wxWindow* parent);

	[[nodiscard]] const auto& getTabName() const { return tabName; }

	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializeConvert();

  private:
	std::string tabName = "Convert";
	std::shared_ptr<Configuration::Configuration> configuration;
	wxStaticText* statusSave;
	wxStaticText* statusConvert;
	wxStaticText* statusCopy;

	void onConvertStarted(wxCommandEvent& event);
};
