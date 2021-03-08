#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"
#include "../../Utils/Localization/Localization.h"
#include <wx/filepicker.h>
#include <wx/notebook.h>

wxDECLARE_EVENT(wxEVT_UPDATEMODS, wxCommandEvent);

class PathsTab final : public wxNotebookPage
{
  public:
	PathsTab(wxWindow* parent);

	void loadConfiguration(const std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializePaths();
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }

  private:
	void OnPathChanged(wxFileDirPickerEvent& evt);

	int pickerCounter = 0;
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;

  protected:
	wxEvtHandler* m_pParent;
};
