#include "LogWindow.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#define tr localization->translate

wxDEFINE_EVENT(wxEVT_PROGRESSMESSAGE, LogMessageEvent);

void FronterGridCellRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, const int row, const int col, bool isSelected)
{
	wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, false);
}

void LogWindow::OnSize(wxSizeEvent& event)
{
	const auto width = GetClientSize().x - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
	theGrid->SetColSize(2, width - theGrid->GetColSize(0) - theGrid->GetColSize(1));
	event.Skip();
}

LogWindow::LogWindow(wxWindow* parent, std::shared_ptr<Localization> theLocalization): wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	m_pParent = parent;
	Bind(wxEVT_TAILTHREAD, &LogWindow::OnTailPush, this);
	localization = std::move(theLocalization);

	theGrid = new wxGrid(this, wxID_ANY);

	theGrid->SetDefaultRenderer(new FronterGridCellRenderer); // We are using our own grid renderer which disables selections.
	
	theGrid->CreateGrid(0, 3, wxGrid::wxGridSelectCells);
	theGrid->EnableEditing(false);
	theGrid->HideCellEditControl();
	theGrid->HideRowLabels();
	theGrid->SetColLabelValue(0, tr("LOGTIME"));
	theGrid->SetColLabelValue(1, tr("LOGSEVERITY"));
	theGrid->SetColLabelValue(2, tr("LOGMESSAGE"));
	theGrid->SetColLabelAlignment(wxLEFT, wxCENTER);
	theGrid->SetScrollRate(0, 20);
	theGrid->SetColLabelSize(20);
	theGrid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_CELL_RIGHT_DCLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_LABEL_LEFT_CLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_LABEL_LEFT_DCLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_LABEL_RIGHT_CLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_LABEL_RIGHT_DCLICK, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_SELECT_CELL, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_GRID_COL_MOVE, &LogWindow::eatClick, this);
	theGrid->Bind(wxEVT_SIZE, &LogWindow::OnSize, this);
	theGrid->DisableDragColSize();
	theGrid->DisableDragRowSize();
	theGrid->DisableDragCell();
	theGrid->DisableDragColMove();
	theGrid->DisableDragGridSize();
	theGrid->DisableCellEditControl();
	theGrid->DisableColResize(0);
	theGrid->DisableColResize(1);
	theGrid->SetColMinimalWidth(0, 150);
	theGrid->SetColMinimalWidth(1, 100);
	theGrid->SetColSize(0, 150);
	theGrid->SetColSize(1, 100);
	
	auto* logBox = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(logBox);
	wxWindowBase::Layout();
	logBox->Fit(this);
	logBox->Add(theGrid, 1, wxEXPAND | wxALL);

	initializeTail();
}

void LogWindow::eatClick(wxGridEvent& event)
{
	//yum.
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

	auto bgcolor = wxColour(0, 0, 0);
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
	theGrid->SetCellValue(logCounter, 1, severity);
	theGrid->SetCellBackgroundColour(logCounter, 1, bgcolor);
	theGrid->SetCellAlignment(logCounter, 1, wxCENTER, wxCENTER);
	theGrid->SetCellValue(logCounter, 2, commonItems::convertUTF8ToUTF16(message));
	theGrid->SetCellBackgroundColour(logCounter, 2, bgcolor);
	theGrid->SetCellAlignment(logCounter, 1, wxLEFT, wxCENTER);
	theGrid->DisableRowResize(logCounter);
	theGrid->HideRow(logCounter);

	auto needUpdate = false;
	if ((logMessage.logLevel == LogLevel::Debug && logLevel >= 3) || (logMessage.logLevel == LogLevel::Info && logLevel >= 2) ||
		 (logMessage.logLevel == LogLevel::Warning && logLevel >= 1) || (logMessage.logLevel == LogLevel::Error))
	{
		theGrid->ShowRow(logCounter);
		needUpdate = true;
	}
	theGrid->EndBatch();

	++logCounter;
	if (needUpdate)
	{
		if (static_cast<int>(message.size()) > maxMessageLength)
		{
			maxMessageLength = static_cast<int>(message.size());
			//theGrid->AutoSizeColumn(2, true); // this is expensive.
		}
		theGrid->Scroll(0, logCounter);
	}
}

void LogWindow::setLogLevel(const int level)
{
	logLevel = level;
	theGrid->BeginBatch();
	for (auto row = 0; row < theGrid->GetNumberRows(); ++row)
	{
		auto value = theGrid->GetCellValue(row, 1);
		if ((value.find("DEBUG") != std::string::npos && logLevel < 3) || (value.find("INFO") != std::string::npos && logLevel < 2) ||
			 (value.find("WARNING") != std::string::npos && logLevel < 1))
			theGrid->HideRow(row);
		else
			theGrid->ShowRow(row);
	}
	theGrid->EndBatch();
	//theGrid->SetColSize(2, 1000);
	// GetParent()->Layout();
	theGrid->Scroll(0, logCounter - 1);
	theGrid->MakeCellVisible(logCounter - 1, 0);
}

void LogWindow::blankLog()
{
	const auto rows = theGrid->GetNumberRows();
	theGrid->DeleteRows(0, rows, true);
	logCounter = 0;	
}