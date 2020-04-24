#include "LogWatcher.h"
#include <fstream>

//DEFINE_EVENT_TYPE(wxEVT_MYTHREAD)

void* MyThread::Entry()
{
	wxCommandEvent evt(wxEVT_MYTHREAD, GetId());
	// can be used to set some identifier for the data
	// evt.SetInt(r);
	// whatever data your thread calculated, to be returned to GUI
	std::string *vm = new std::string();
	*vm = "no";
	evt.SetClientData((void*)vm);
	wxPostEvent(m_pParent, evt);

	while (true)
	{
		auto linesreadsofar = linesRead
		std::ifstream logfile("log.txt", std::ifstream::in);
		for (auto i = 0; i < linesRead; i++)
		{
			std::string line;
			std::getline(logfile,line);
			linesRead++;
		}
	}

	
	return 0;
}