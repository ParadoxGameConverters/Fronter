#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"
#include "../../Utils/Localization/Localization.h"

class ModsTab final: public wxScrolledWindow
{
  public:
	explicit ModsTab(wxWindow* parent);

	void loadConfiguration(const std::shared_ptr<Configuration>& theConfiguration) { configuration = theConfiguration; }
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }

	void initializeMods();

  private:
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;

	wxStaticText* title = nullptr;
	std::vector<wxCheckBox*> checkBoxes;
	wxBoxSizer* modsTabSizer;

  protected:
	wxEvtHandler* m_pParent;
};
