#include "LogWindow.h"
#include "Log.h"
#include "../LogWatcher/LogWatcher.h"

BEGIN_EVENT_TABLE(LogWindow,wxScrolledWindow)
    EVT_COMMAND(wxID_ANY, wxEVT_MYTHREAD, LogWindow::OnMyThread)
END_EVENT_TABLE()

LogWindow::LogWindow(wxWindow* parent, const wxPoint& pos, const wxSize& size): wxScrolledWindow(parent, wxID_ANY, pos, size)
{
	PerformCalculation(2);
}

void LogWindow::PerformCalculation(int someParameter){
    //create the thread
    MyThread *thread = new MyThread(this, someParameter);
    thread->Create();
    thread->Run();
    //Don't worry about deleting the thread, there are two types of wxThreads 
    //and this kind deletes itself when it's finished.
}
void LogWindow::OnMyThread(wxCommandEvent& event)
{
    std::string* temp = (std::string*)event.GetClientData();
    //do something with temp, which holds unsigned char* data from the thread
    //GetClientData() can return any kind of data you want, but you have to cast it.
    //auto temp = event.GetId();
    Log(LogLevel::Debug) << "got data |" << *temp << "|";
    delete temp; 
}    