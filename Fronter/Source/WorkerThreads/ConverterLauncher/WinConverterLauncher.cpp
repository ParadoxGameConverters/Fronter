#include "ConverterLauncher.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <winbase.h>
#include <windows.h>
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
	const auto converterExeString = exeItr->second->getValue();
	if (converterExeString.empty())
	{
		Log(LogLevel::Error) << "Converter location has not been set!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}
	if (!commonItems::DoesFileExist(converterExeString))
	{
		Log(LogLevel::Error) << "Could not find converter executable!";
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}

	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {nullptr};

	const auto pos = converterExeString.find_last_of('\\');
	const auto workDirString = converterExeString.substr(0, pos + 1);
	const auto converterExe = commonItems::convertUTF8ToUTF16(converterExeString);
	const auto workDir = commonItems::convertUTF8ToUTF16(workDirString);
	const wchar_t* workDirPtr = workDir.c_str();
	const auto stopWatchStart = std::chrono::steady_clock::now();

	if (CreateProcess(converterExe.c_str(), // No module name (use command line)
			  nullptr,								 // Command line
			  nullptr,								 // Process handle not inheritable
			  nullptr,								 // Thread handle not inheritable
			  FALSE,									 // Set handle inheritance to FALSE
			  CREATE_NO_WINDOW,					 // No creation flags
			  nullptr,								 // Use parent's environment block
			  workDirPtr,							 // Use parent's starting directory
			  &si,									 // Pointer to STARTUPINFO structure
			  &pi))
	{
		DWORD exit_code;
		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &exit_code);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		const auto stopWatchEnd = std::chrono::steady_clock::now();
		Log(LogLevel::Info) << "Converter exited at: " << std::chrono::duration_cast<std::chrono::seconds>(stopWatchEnd - stopWatchStart).count() << " seconds.";
		if (exit_code > 0)
		{
			wxMilliSleep(100); // waiting on second tail to finish transcribing.
			Log(LogLevel::Error) << "Converter Error! See log.txt for details.";
			Log(LogLevel::Error) << "If you require assistance please upload log.txt to forums for a detailed post-mortem.";
			evt.SetInt(0);
			m_pParent->AddPendingEvent(evt);
			return nullptr;
		}
	}
	else
	{
		Log(LogLevel::Error) << "Could not execute converter: " << std::to_string(GetLastError()) << " " << commonItems::GetLastErrorString();
		evt.SetInt(0);
		m_pParent->AddPendingEvent(evt);
		return nullptr;
	}

	evt.SetInt(1);
	m_pParent->AddPendingEvent(evt);
	return nullptr;
}
