#include "LogWatcher.h"
#include "Log.h"
#include "LogMessageEvent.h"
#include <fstream>

wxDEFINE_EVENT(wxEVT_TAILTHREAD, LogMessageEvent);

void* LogWatcher::Entry()
{
	LogMessageEvent evt(wxEVT_TAILTHREAD);
	std::ifstream logfile(tailSource, std::ifstream::in);

	std::string line;
	auto oneLastRun = false;
	while (!terminate || oneLastRun)
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
					m_pParent->QueueEvent(evt.Clone());
				}
			}
		}
		wxMilliSleep(50);
		logfile.clear();
		if (terminate && oneLastRun)
			break;
		if (terminate)
			oneLastRun = true;
	}
	logfile.close();

	return nullptr;
}