#include "LogWatcher.h"
#include "Log.h"
#include <fstream>
#include "LogMessageEvent.h"

wxDEFINE_EVENT(wxEVT_TAILTHREAD, LogMessageEvent);

void* LogWatcher::Entry()
{
	LogMessageEvent evt(wxEVT_TAILTHREAD);

	std::ifstream logfile(tailSource, std::ifstream::in);

	std::string line;
	while (!terminate)
	{
		while (logfile)
		{
			if (std::getline(logfile, line))
			{
				auto logMessage = sliceMessage(line);
				if (transcriberMode)
				{
					Log(logMessage.logLevel) << logMessage.message;
				}
				if (emitterMode)
				{
					evt.SetMessage(logMessage);
					m_pParent->AddPendingEvent(evt);
				}
			}
		}
		wxMilliSleep(300);
		logfile.clear();
	}
	logfile.close();

	return 0;
}