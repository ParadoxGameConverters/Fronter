#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/thread.h>

class LogWatcher final: public wxThread
{
  public:
	explicit LogWatcher(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}
	void terminateTail() { terminate = true; }
	void setTailSource(const std::string& theSource) { tailSource = theSource; }
	void setTranscriberMode(const bool mode) { transcriberMode = mode; }
	void setEmitterMode(const bool mode) { emitterMode = mode; }

  private:
	void* Entry() override;
	bool terminate = false;
	std::string tailSource;
	bool transcriberMode = false;
	bool emitterMode = false;

  protected:
	wxEvtHandler* m_pParent;
};
