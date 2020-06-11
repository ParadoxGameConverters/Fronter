#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"
#include "../../Utils/Localization/Localization.h"

class ModsTab: public wxScrolledWindow
{
  public:
	ModsTab(wxWindow* parent);

	void loadConfiguration(std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }

	void initializeMods();
	
  private:
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;

	wxStaticText* title = nullptr;
	wxWindow* boxHolder = nullptr;
	std::vector<wxCheckBox*> checkBoxes;
	wxBoxSizer* modsTabSizer;
	wxBoxSizer* boxSizer;
	
  protected:
	wxEvtHandler* m_pParent;
};
