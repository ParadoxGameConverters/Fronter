#pragma once
#include "../WorkerThreads/LogWatcher/LogWatcher.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../Utils/Localization/Localization.h"
#include "../WorkerThreads/LogWatcher/LogMessageEvent.h"
#include <wx/grid.h>
#include <memory>

class LogWindow: public wxWindow
{
  public:
	LogWindow(wxWindow* parent, std::shared_ptr<Localization> theLocalization);

	void initializeTail();
	void initializeSecondTail(const std::string& tailSource);
	void terminateSecondTail() const;
	void OnTailPush(LogMessageEvent& event);
	void setLogLevel(int level);

  private:
	LogWatcher* logWatcher = nullptr;
	LogWatcher* logWatcher2 = nullptr;
	int logCounter = 0;
	wxGrid* theGrid = nullptr;
	std::vector<wxStaticText*> logArray;
	std::shared_ptr<Localization> localization;
	int loglevel = 1; // Defaults to warning.

  protected:
	wxEvtHandler* m_pParent;
};
