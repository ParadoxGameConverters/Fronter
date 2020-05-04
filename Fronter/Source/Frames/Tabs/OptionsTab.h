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

	void loadConfiguration(std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }
	void initializeOptions();

  private:
	std::shared_ptr<Configuration> configuration;
};
