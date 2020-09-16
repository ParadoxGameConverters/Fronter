#include "ConverterLauncher.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
namespace fs = std::filesystem;

wxDEFINE_EVENT(wxEVT_CONVERTERDONE, wxCommandEvent);

void* ConverterLauncher::Entry()
{
	wxCommandEvent evt(wxEVT_CONVERTERDONE);

	const auto reqFiles = configuration->getRequiredFiles();
	const auto& exeItr = reqFiles.find("converterExe");
	if (exeItr == reqFiles.end())
	{
		Log(LogLevel::Error) << "Converter location has not been loaded!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	const auto converterExe = exeItr->second->getValue();
	if (converterExe.empty())
	{
		Log(LogLevel::Error) << "Converter location has not been set!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	if (!fs::exists(fs::u8path(converterExe)))
	{
		Log(LogLevel::Error) << "Could not find converter executable!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}

	const auto pos = converterExe.find_last_of('/');
	const auto workDir = converterExe.substr(0, pos + 1);
	const auto stopWatchStart = std::chrono::steady_clock::now();

	auto exeCommand = "cd " + workDir + "; " + converterExe;
	const char* exeCommandChar = exeCommand.c_str();

	auto result = system(exeCommandChar);
    const auto stopWatchEnd = std::chrono::steady_clock::now();
	if (result > 0)
	{
		evt.SetInt(1);
		Log(LogLevel::Info) << "Converter finished at: " << std::chrono::duration_cast<std::chrono::seconds>(stopWatchEnd - stopWatchStart).count() << " seconds.";
	}
	else
	{
		evt.SetInt(0);
		Log(LogLevel::Error) << "Could not execute converter!";
	}
	
	m_pParent->AddPendingEvent(evt);
	return nullptr;
}
