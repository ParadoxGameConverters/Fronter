#include "LogWatcher.h"
#include "Log.h"
#include <fstream>

DEFINE_EVENT_TYPE(wxEVT_TAILTHREAD)

void* LogWatcher::Entry()
{
	wxCommandEvent evt(wxEVT_TAILTHREAD, GetId());

	std::ifstream logfile(tailSource, std::ifstream::in);

	std::string line;
	while (!terminate)
	{
		while (logfile)
		{
			if (std::getline(logfile, line))
			{
				evt.SetString(line);
				m_pParent->AddPendingEvent(evt);
			}
		}
		wxMilliSleep(300);
		logfile.clear();
	}
	logfile.close();

	return 0;
}