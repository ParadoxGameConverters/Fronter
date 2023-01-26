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
#include <wx/gauge.h>
#include <wx/notebook.h>

class MainFrame;
wxDECLARE_EVENT(wxEVT_LOGLEVELCHANGED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_BLANKLOG, wxCommandEvent);

class ConvertTab final: public wxNotebookPage
{
  public:
	explicit ConvertTab(wxWindow* parent);

	void loadConfiguration(const std::shared_ptr<Configuration>& theConfiguration) { configuration = theConfiguration; }
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }
	void initializeConvert();
	void loadSelf(MainFrame* theMainFrame) { mainFrame = theMainFrame; }
	void setProgress(int progress);


  private:
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;

	wxStaticText* statusSave = nullptr;
	wxStaticText* statusConvert = nullptr;
	wxStaticText* statusCopy = nullptr;
	MainFrame* mainFrame = nullptr;
	ConverterLauncher* converterLauncher = nullptr;
	ModCopier* modCopier = nullptr;
	wxButton* convertButton = nullptr;
	wxGauge* gauge = nullptr;
	wxStaticText* gaugeCounter = nullptr;

	void onConvertStarted(wxCommandEvent& event);
	void onConverterDone(wxCommandEvent& event);
	void onCopierDone(wxCommandEvent& event);

  protected:
	wxEvtHandler* m_pParent;
};
