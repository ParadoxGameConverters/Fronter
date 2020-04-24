#include "LogWatcher.h"
#include <fstream>
#include "Log.h"

DEFINE_EVENT_TYPE(wxEVT_TAILTHREAD)

void* LogWatcher::Entry()
{
	wxCommandEvent evt(wxEVT_TAILTHREAD, GetId());
	
	std::string* lineptr = new std::string();
	std::ifstream logfile("log.txt", std::ifstream::in);

	while (!terminate)
	{
		std::string line;
		while (std::getline(logfile, line))
		{
			*lineptr = line;
			evt.SetClientData(static_cast<void*>(lineptr));
			wxPostEvent(m_pParent, evt);
			linesRead++;
		}
		wxMilliSleep(500);
		logfile.clear();
	}
	logfile.close();

	
	return 0;
}