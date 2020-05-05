#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"
#include "../../Utils/Localization/Localization.h"

class OptionsTab: public wxScrolledWindow
{
  public:
	OptionsTab(wxWindow* parent);

	void loadConfiguration(std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializeOptions();
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }

  private:
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;
};
