#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"
#include "../../Utils/Localization/Localization.h"
#include "../../WorkerThreads/ConverterLauncher/ConverterLauncher.h"
#include "../../WorkerThreads/ModCopier/ModCopier.h"
#include <wx/filepicker.h>
#include <wx/notebook.h>

class MainFrame;

class ConvertTab: public wxNotebookPage
{
  public:
	ConvertTab(wxWindow* parent);

	[[nodiscard]] const auto& getTabName() const { return tabName; }

	void loadConfiguration(std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }
	void initializeConvert();
	void loadSelf(MainFrame* theMainFrame) { mainFrame = theMainFrame; }


  private:
	std::string tabName = "Convert";
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;

	wxStaticText* statusSave = nullptr;
	wxStaticText* statusConvert = nullptr;
	wxStaticText* statusCopy = nullptr;
	MainFrame* mainFrame = nullptr;
	ConverterLauncher* converterLauncher = nullptr;
	ModCopier* modCopier = nullptr;
	wxButton* convertButton = nullptr;

	void onConvertStarted(wxCommandEvent& event);
	void onConverterDone(wxCommandEvent& event);
	void onCopierDone(wxCommandEvent& event);
};
