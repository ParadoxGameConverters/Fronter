#include "ConverterLauncher.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <winbase.h>
#include <windows.h>
namespace fs = std::filesystem;

DEFINE_EVENT_TYPE(wxEVT_CONVERTERDONE)

void* ConverterLauncher::Entry()
{
	wxCommandEvent evt(wxEVT_CONVERTERDONE, GetId());

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

	STARTUPINFOA si = {0};
	PROCESS_INFORMATION pi = {0};

	const auto pos = converterExe.find_last_of('\\');
	const auto workDir = converterExe.substr(0, pos + 1);
	const char* workDirPtr = workDir.c_str();
	const auto stopWatchStart = std::chrono::steady_clock::now();

	if (CreateProcessA(converterExe.c_str(), // No module name (use command line)
			  nullptr,								  // Command line
			  nullptr,								  // Process handle not inheritable
			  nullptr,								  // Thread handle not inheritable
			  FALSE,									  // Set handle inheritance to FALSE
			  CREATE_NO_WINDOW,					  // No creation flags
			  nullptr,								  // Use parent's environment block
			  workDirPtr,							  // Use parent's starting directory
			  &si,									  // Pointer to STARTUPINFO structure
			  &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		const auto stopWatchEnd = std::chrono::steady_clock::now();
		Log(LogLevel::Info) << "Converter finished at: " << std::chrono::duration_cast<std::chrono::seconds>(stopWatchEnd - stopWatchStart).count()
								  << " seconds.";
	}
	else
	{
		Log(LogLevel::Error) << "Could not execute converter: " << std::to_string(GetLastError()) << " " << Utils::GetLastErrorString();
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}

	evt.SetInt(1);
	m_pParent->AddPendingEvent(evt);
	return nullptr;
}
