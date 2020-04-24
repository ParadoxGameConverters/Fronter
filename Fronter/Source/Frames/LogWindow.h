#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class LogWindow : public wxScrolledWindow
{
public:
	LogWindow(wxWindow* parent, const wxPoint& pos, const wxSize& size);

	
private:
   //wxDECLARE_EVENT_TABLE();

};