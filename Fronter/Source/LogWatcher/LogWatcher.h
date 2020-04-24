#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/thread.h>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_TAILTHREAD, -1)
END_DECLARE_EVENT_TYPES()

class LogWatcher: public wxThread
{
  public:
	LogWatcher(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}
	void terminateTail() { terminate = true; }
	
  private:
	void* Entry();
	int linesRead = 0;
	bool terminate = false;

  protected:
	wxEvtHandler* m_pParent;
};
