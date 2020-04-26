#pragma once
#include <wx/wxprec.h>
#include "../LogWatcher/LogWatcher.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class LogWindow: public wxScrolledWindow
{
  public:
	LogWindow(wxWindow* parent);

	void initializeTail();
	void initializeSecondTail(const std::string& tailSource);
	void terminateSecondTail() const;
	void OnTailPush(wxCommandEvent& event);
	
  private:
	LogWatcher* logWatcher = nullptr;
	LogWatcher* logWatcher2 = nullptr;
	wxDECLARE_EVENT_TABLE();
	int logCounter = 0;
};