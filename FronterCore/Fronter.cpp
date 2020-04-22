// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".


#include "Fronter.h"
#include "MainFrame.h"


wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
   MyFrame* frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(1200, 600));
   frame->Show(true);
   return true;
}
