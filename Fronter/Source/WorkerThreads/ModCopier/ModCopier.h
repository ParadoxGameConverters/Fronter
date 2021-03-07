#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../../Configuration/Configuration.h"
#include <wx/thread.h>

wxDECLARE_EVENT(wxEVT_COPIERDONE, wxCommandEvent);

class ModCopier final : public wxThread
{
  public:
	ModCopier(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}
	void loadConfiguration(const std::shared_ptr<Configuration> theConfiguration) { configuration = theConfiguration; }

  private:
	void* Entry() override;
	std::shared_ptr<Configuration> configuration;

  protected:
	wxEvtHandler* m_pParent;
};
