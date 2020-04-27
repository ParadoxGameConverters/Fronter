#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"

class OptionsTab: public wxScrolledWindow
{
  public:
	OptionsTab(wxWindow* parent);

	[[nodiscard]] const auto& getTabName() const { return tabName; }

	void loadConfiguration(std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializeOptions();

  private:
	std::string tabName = "Options";
	std::shared_ptr<Configuration> configuration;
};
