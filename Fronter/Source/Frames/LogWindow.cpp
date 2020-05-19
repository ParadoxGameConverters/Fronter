#include "LogWindow.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#define tr localization->translate

wxDEFINE_EVENT(wxEVT_PROGRESSMESSAGE, LogMessageEvent);

LogWindow::LogWindow(wxWindow* parent, std::shared_ptr<Localization> theLocalization): wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	m_pParent = parent;
	Bind(wxEVT_TAILTHREAD, &LogWindow::OnTailPush, this);
	localization = std::move(theLocalization);
	theGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);
	theGrid->CreateGrid(0, 3, wxGrid::wxGridSelectCells);
	theGrid->HideCellEditControl();
	theGrid->HideRowLabels();
	theGrid->SetColLabelValue(0, tr("LOGTIME"));
	theGrid->SetColLabelValue(1, tr("LOGSEVERITY"));
	theGrid->SetColLabelValue(2, tr("LOGMESSAGE"));
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
	else if (logMessage.logLevel == LogLevel::Progress)
	{
		// Forward the progress message, stripped version, to parent.
		try
		{
			auto progress = std::stoi(logMessage.message);
			LogMessageEvent evt(wxEVT_PROGRESSMESSAGE);
			evt.SetInt(progress);
			m_pParent->AddPendingEvent(evt);
			return;
		}
		catch (std::exception&)
		{
			return;
		}
	}

	const auto message = "  " + logMessage.message;

	theGrid->BeginBatch();
	theGrid->AppendRows(1, false);
	theGrid->SetRowSize(logCounter, 20);
	theGrid->SetCellValue(logCounter, 0, timestamp);
	theGrid->SetCellAlignment(logCounter, 0, wxCENTER, wxCENTER);
	theGrid->SetReadOnly(logCounter, 0);
	theGrid->SetCellValue(logCounter, 1, severity);
	theGrid->SetCellBackgroundColour(logCounter, 1, bgcolor);
	theGrid->SetCellAlignment(logCounter, 1, wxCENTER, wxCENTER);
	theGrid->SetReadOnly(logCounter, 1);
	theGrid->SetCellValue(logCounter, 2, Utils::convertUTF8ToUTF16(message));
	theGrid->SetCellBackgroundColour(logCounter, 2, bgcolor);
	theGrid->SetCellAlignment(logCounter, 1, wxLEFT, wxCENTER);
	theGrid->SetReadOnly(logCounter, 2);
	theGrid->HideRow(logCounter);

	auto needUpdate = false;
	if (logMessage.logLevel == LogLevel::Debug && loglevel >= 3 || logMessage.logLevel == LogLevel::Info && loglevel >= 2 ||
		 logMessage.logLevel == LogLevel::Warning && loglevel >= 1 || logMessage.logLevel == LogLevel::Error)
	{
		theGrid->ShowRow(logCounter);
		needUpdate = true;
	}
	theGrid->EndBatch();
	
	logCounter++;
	if (needUpdate)
	{
		theGrid->AutoSize();
		GetParent()->Layout();
		theGrid->Scroll(0, logCounter - 1);
		theGrid->MakeCellVisible(logCounter - 1, 0);
	}
}

void LogWindow::setLogLevel(int level)
{
	loglevel = level;
	theGrid->BeginBatch();
	for (int row = 0; row < theGrid->GetNumberRows(); row++)
	{
		auto value = theGrid->GetCellValue(row, 1);
		if (value.find("DEBUG") != std::string::npos)
			if (loglevel < 3)
				theGrid->HideRow(row);
			else
				theGrid->ShowRow(row);
		else if (value.find("INFO") != std::string::npos)
			if (loglevel < 2)
				theGrid->HideRow(row);
			else
				theGrid->ShowRow(row);
		else if (value.find("WARNING") != std::string::npos)
			if (loglevel < 1)
				theGrid->HideRow(row);
			else
				theGrid->ShowRow(row);
	}
	theGrid->EndBatch();
	theGrid->AutoSize();
	GetParent()->Layout();
	theGrid->Scroll(0, logCounter - 1);
	theGrid->MakeCellVisible(logCounter - 1, 0);
}
