#include "LogWindow.h"
#include "Log.h"

LogWindow::LogWindow(wxWindow* parent): wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, -1))
{
	Bind(wxEVT_TAILTHREAD, &LogWindow::OnTailPush, this);
	
	SetScrollRate(0, 20);
	wxBoxSizer* logBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(logBox);
	initializeTail();
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
	logCounter++;
	const auto logMessage = event.GetMessage();
	wxStaticText* st = new wxStaticText(this, wxID_ANY, logMessage.timestamp + " " + logMessage.message, wxDefaultPosition, wxSize(-1, 18));
	GetSizer()->Add(st, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER | wxEXPAND | wxALL, 1);
	GetParent()->Layout();
	Scroll(0, logCounter);
}