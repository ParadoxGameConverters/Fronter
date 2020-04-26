#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/thread.h>
#include "../ConfigurationParser/Configuration.h"

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_CONVERTERDONE, wxID_ANY)
END_DECLARE_EVENT_TYPES()

class ConverterLauncher: public wxThread
{
  public:
	ConverterLauncher(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}
	void loadConfiguration(std::shared_ptr<Configuration::Configuration> theConfiguration) { configuration = theConfiguration; }

  private:
	void* Entry();
	std::shared_ptr<Configuration::Configuration> configuration;

  protected:
	wxEvtHandler* m_pParent;
};
