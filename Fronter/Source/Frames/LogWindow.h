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
	void OnTailPush(LogMessageEvent& event);
	
  private:
	LogWatcher* logWatcher = nullptr;
	LogWatcher* logWatcher2 = nullptr;
	int logCounter = 0;
};