#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Utils/SliceMessage.h"

class LogMessageEvent;
wxDECLARE_EVENT(wxEVT_TAILTHREAD, LogMessageEvent);
wxDECLARE_EVENT(wxEVT_PROGRESSMESSAGE, LogMessageEvent);

typedef void (wxEvtHandler::*LogMessageEventFunction)(LogMessageEvent&);
#define LogMessageEventHandler(func) wxEVENT_HANDLER_CAST(LogMessageEventFunction, func)

class LogMessageEvent final : public wxCommandEvent
{
  public:
	explicit LogMessageEvent(const wxEventType commandType = wxEVT_TAILTHREAD): wxCommandEvent(commandType) {}

	// You *must* copy here the data to be transported
	LogMessageEvent(const LogMessageEvent& event): wxCommandEvent(event) { this->SetMessage(event.GetMessage()); }

	// Required for sending with wxPostEvent()
	[[nodiscard]] wxEvent* Clone() const override { return new LogMessageEvent(*this); }

	[[nodiscard]] LogMessage GetMessage() const { return logMessage; }
	void SetMessage(const LogMessage& message) { logMessage = message; }

  private:
	LogMessage logMessage;
};
