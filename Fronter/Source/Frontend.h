#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Configuration/Configuration.h"
#include "Utils/Localization/Localization.h"

class Frontend final: public wxApp
{
  public:
	bool OnInit() override;
	std::shared_ptr<Configuration> configuration;
	std::shared_ptr<Localization> localization;
};