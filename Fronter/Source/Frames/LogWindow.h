#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class LogWindow: public wxScrolledWindow
{
  public:
	LogWindow(wxWindow* parent);

	void initializeTail();
	void OnTailPush(wxCommandEvent& event);

  private:
	wxDECLARE_EVENT_TABLE();
	int logCounter = 0;
};