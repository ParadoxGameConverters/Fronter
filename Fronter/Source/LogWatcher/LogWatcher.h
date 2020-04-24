#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/thread.h>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_MYTHREAD, -1)
END_DECLARE_EVENT_TYPES()

class MyThread: public wxThread
{
  public:
	MyThread(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent){}

  private:
	void* Entry();
	int linesRead = 0;

  protected:
	wxEvtHandler* m_pParent;
};
