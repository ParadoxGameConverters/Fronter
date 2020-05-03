#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Configuration/Configuration.h"
#include "Utils/Localization/Localization.h"

class Frontend: public wxApp
{
  public:
	virtual bool OnInit();
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;
};