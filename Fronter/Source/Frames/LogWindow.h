#pragma once
#include "../WorkerThreads/LogWatcher/LogWatcher.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../Utils/Localization/Localization.h"
#include "../WorkerThreads/LogWatcher/LogMessageEvent.h"
#include <memory>
#include <wx/grid.h>

class FronterGridCellRenderer final: public wxGridCellStringRenderer
{
  public:
	void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) override;
};

class LogWindow final: public wxWindow
{
  public:
	LogWindow(wxWindow* parent, std::shared_ptr<Localization> theLocalization);

	void initializeTail();
	void initializeSecondTail(const std::string& tailSource);
	void terminateSecondTail() const;
	void OnTailPush(LogMessageEvent& event);
	void setLogLevel(int level);
	void blankLog();

  private:
	void eatClick(wxGridEvent& event);
	void OnSize(wxSizeEvent& event);

	LogWatcher* logWatcher = nullptr;
	LogWatcher* logWatcher2 = nullptr;
	int logCounter = 0;
	wxGrid* theGrid = nullptr;
	std::vector<wxStaticText*> logArray;
	std::shared_ptr<Localization> localization;
	int logLevel = 1; // Defaults to warning.
	int maxMessageLength = 0;

  protected:
	wxEvtHandler* m_pParent;
};
