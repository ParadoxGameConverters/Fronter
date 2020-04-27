#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../Configuration/Configuration.h"
#include <wx/thread.h>
#include "LogMessageEvent.h"

class LogWatcher: public wxThread
{
  public:
	LogWatcher(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}
	void terminateTail() { terminate = true; }
	void setTailSource(const std::string& theSource) { tailSource = theSource; }
	void setTranscriberMode(bool mode) { transcriberMode = mode;}
	void setEmitterMode(bool mode) { emitterMode = mode;}
	
  private:
	void* Entry();
	bool terminate = false;
	std::string tailSource;
	bool transcriberMode = false;
	bool emitterMode = false;

  protected:
	wxEvtHandler* m_pParent;
};
