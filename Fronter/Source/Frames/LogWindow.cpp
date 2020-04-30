#include "LogWindow.h"
#include "Log.h"

LogWindow::LogWindow(wxWindow* parent): wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, -1))
{
	Bind(wxEVT_TAILTHREAD, &LogWindow::OnTailPush, this);

	SetScrollRate(0, 20);
	wxFlexGridSizer* logBox = new wxFlexGridSizer(3);
	SetSizer(logBox);
	initializeTail();
	logBox->AddGrowableCol(2);
}

void LogWindow::initializeTail()
{
	logWatcher = new LogWatcher(this);
	logWatcher->Create();
	logWatcher->setEmitterMode(true);
	logWatcher->setTailSource("log.txt");
	logWatcher->Run();
}

void LogWindow::initializeSecondTail(const std::string& tailSource)
{
	logWatcher2 = new LogWatcher(this);
	logWatcher2->Create();
	logWatcher2->setTranscriberMode(true);
	logWatcher2->setTailSource(tailSource);
	logWatcher2->Run();
}

void LogWindow::terminateSecondTail() const
{
	logWatcher2->terminateTail();
}

void LogWindow::OnTailPush(LogMessageEvent& event)
{
	// Limiting to 300 cells for performance reasons
	if (logCounter >= 100)
		for (auto counter = (logCounter - 100) * 3; counter < (logCounter - 100) * 3 + 3; counter++)
		{
			logArray[counter]->Destroy();
		}
	GetParent()->Layout();
	
	logCounter++;
	const auto logMessage = event.GetMessage();
	wxStaticText* timestamp = new wxStaticText(this, wxID_ANY, "  " + logMessage.timestamp + "  ", wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	timestamp->SetBackgroundColour(0x00FFFFFF);
	timestamp->SetMinSize(wxSize(-1, 20));
	GetSizer()->Add(timestamp, 1, wxALIGN_CENTER);
	logArray.emplace_back(timestamp);

	wxColour bgcolor = wxColour(0, 0, 0);
	std::string severity;
	if (logMessage.logLevel == LogLevel::Info)
	{
		bgcolor = wxColour(255, 255, 255);
		severity = "  INFO  ";
	}
	else if (logMessage.logLevel == LogLevel::Debug)
	{
		bgcolor = wxColour(200, 200, 200);
		severity = "  DEBUG  ";
	}
	else if (logMessage.logLevel == LogLevel::Warning)
	{
		bgcolor = wxColour(255, 255, 200);
		severity = "  WARNING  ";
	}
	else if (logMessage.logLevel == LogLevel::Error)
	{
		bgcolor = wxColour(255, 200, 200);
		severity = "  ERROR  ";
	}
	wxStaticText* sev = new wxStaticText(this, wxID_ANY, severity, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE | wxALIGN_CENTRE_HORIZONTAL);
	sev->SetBackgroundColour(bgcolor);
	GetSizer()->Add(sev, 1, wxCENTER | wxEXPAND );
	logArray.emplace_back(sev);
	
	wxStaticText* st = new wxStaticText(this, wxID_ANY, "  " + logMessage.message + "  ", wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	st->SetBackgroundColour(bgcolor);
	GetSizer()->Add(st, 1, wxALIGN_LEFT | wxEXPAND);
	logArray.emplace_back(st);
	
	GetParent()->Layout();
	Scroll(0, logCounter - 1);
}