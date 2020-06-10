#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"
#include "../../Utils/Localization/Localization.h"
#include "OptionBox.h"
#include <wx/wrapsizer.h>

class OptionsTab: public wxScrolledWindow
{
  public:
	OptionsTab(wxWindow* parent);

	void loadConfiguration(std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializeOptions();
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }
	void createModOptionBox();

  private:
	void fillModOption();

	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;
	std::shared_ptr<Option> modOption;
	OptionBox* modOptionBox = nullptr;
	wxWrapSizer* optionsTabSizer;
};
