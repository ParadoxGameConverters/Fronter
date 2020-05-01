#include "LogWindow.h"
#include "Log.h"

LogWindow::LogWindow(wxWindow* parent): wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	Bind(wxEVT_TAILTHREAD, &LogWindow::OnTailPush, this);

	theGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);
	theGrid->CreateGrid(0, 3, wxGrid::wxGridSelectCells);
	theGrid->HideCellEditControl();
	theGrid->HideRowLabels();
	theGrid->SetColLabelValue(0, "Timestamp");
	theGrid->SetColLabelValue(1, "Vehemence");
	theGrid->SetColLabelValue(2, "Message");
	theGrid->SetColLabelAlignment(wxLEFT, wxCENTER);
	theGrid->SetScrollRate(0, 20);
	theGrid->SetColLabelSize(20);
	
	wxBoxSizer* logBox = new wxBoxSizer(wxVERTICAL);
	logBox->Add(theGrid, wxSizerFlags(1).Expand());
	SetSizer(logBox);
	logBox->Fit(this);
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
	const auto logMessage = event.GetMessage();	
	const auto timestamp = "  " + logMessage.timestamp + "  ";

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

	const auto message = "  " + logMessage.message;

	theGrid->AppendRows(1, false);
	theGrid->SetRowSize(logCounter, 20);
	theGrid->SetCellValue(logCounter, 0, timestamp);
	theGrid->SetCellAlignment(logCounter, 0, wxCENTER, wxCENTER);
	theGrid->SetReadOnly(logCounter, 0);
	theGrid->SetCellValue(logCounter, 1, severity);
	theGrid->SetCellBackgroundColour(logCounter, 1, bgcolor);
	theGrid->SetCellAlignment(logCounter, 1, wxCENTER, wxCENTER);
	theGrid->SetReadOnly(logCounter, 1);
	theGrid->SetCellValue(logCounter, 2, message);
	theGrid->SetCellBackgroundColour(logCounter, 2, bgcolor);
	theGrid->SetCellAlignment(logCounter, 1, wxLEFT, wxCENTER);
	theGrid->SetReadOnly(logCounter, 2);
	logCounter++;
	theGrid->AutoSize();
	GetParent()->Layout();
	theGrid->Scroll(0, logCounter - 1);
	theGrid->MakeCellVisible(logCounter - 1, 0);
}