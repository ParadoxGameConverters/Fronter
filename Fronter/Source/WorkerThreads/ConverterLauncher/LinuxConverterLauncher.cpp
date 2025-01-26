#include "CommonFunctions.h"
#include "ConverterLauncher.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>



wxDEFINE_EVENT(wxEVT_CONVERTERDONE, wxCommandEvent);



void* ConverterLauncher::Entry()
{
	wxCommandEvent evt(wxEVT_CONVERTERDONE);

	auto converterFolder = configuration->getConverterFolder();
	auto backendExePath = configuration->getBackendExePath();
	auto backendExePathRelativeToFrontend = converterFolder / backendExePath;
	auto backendExePathString = backendExePathRelativeToFrontend.string();

	if (backendExePath.empty())
	{
		Log(LogLevel::Error) << "Converter location has not been set!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	if (!commonItems::DoesFileExist(backendExePathRelativeToFrontend))
	{
		Log(LogLevel::Error) << "Could not find converter executable!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}

	const auto backendExeName = backendExePathRelativeToFrontend.filename();
	const auto workDir = backendExePathRelativeToFrontend.parent_path();
	const auto stopWatchStart = std::chrono::steady_clock::now();

	auto exeCommand = "cd " + workDir.string() + "; ./" + backendExeName.string();
	const char* exeCommandChar = exeCommand.c_str();

	auto result = system(exeCommandChar);
	const auto stopWatchEnd = std::chrono::steady_clock::now();
	if (result > 0)
	{
		evt.SetInt(1);
		Log(LogLevel::Info) << "Converter finished at: " << std::chrono::duration_cast<std::chrono::seconds>(stopWatchEnd - stopWatchStart).count()
								  << " seconds.";
	}
	else
	{
		evt.SetInt(0);
		Log(LogLevel::Error) << "Could not execute converter!";
	}

	m_pParent->AddPendingEvent(evt);
	return nullptr;
}
