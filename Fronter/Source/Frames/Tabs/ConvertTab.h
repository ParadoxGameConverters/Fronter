#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../ConfigurationParser/Configuration.h"
#include <wx/filepicker.h>
#include <wx/notebook.h>
#include "../../ConverterLauncher/ConverterLauncher.h"


class MainFrame;

class ConvertTab: public wxNotebookPage
{
  public:
	ConvertTab(wxWindow* parent);

	[[nodiscard]] const auto& getTabName() const { return tabName; }

	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializeConvert();
	void loadSelf(MainFrame* theMainFrame) { mainFrame = theMainFrame; }

	void onConverterDone(wxCommandEvent& event);
	
  private:
	std::string tabName = "Convert";
	std::shared_ptr<Configuration::Configuration> configuration;
	wxStaticText* statusSave = nullptr;
	wxStaticText* statusConvert = nullptr;
	wxStaticText* statusCopy = nullptr;
	MainFrame* mainFrame = nullptr;
	ConverterLauncher* converterLauncher = nullptr;
	wxButton* convertButton = nullptr;
	wxDECLARE_EVENT_TABLE();
	void onConvertStarted(wxCommandEvent& event);
};
