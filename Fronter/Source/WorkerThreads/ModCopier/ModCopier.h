#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "SQLiteCpp/SQLiteCpp.h"
#include "../../Configuration/Configuration.h"
#include <wx/thread.h>

wxDECLARE_EVENT(wxEVT_COPIERDONE, wxCommandEvent);

class ModCopier final: public wxThread
{
  public:
	explicit ModCopier(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}
	void loadConfiguration(const std::shared_ptr<Configuration>& theConfiguration) { configuration = theConfiguration; }

  private:
	void* Entry() override;
	void CreatePlayset(const std::string& destModFolder, const std::string& targetName, const std::string& destModFolderPath);
	[[nodiscard]] std::string GetLastUpdatedLauncherDbPath(const std::string& gameDocsDirectory) const;
	void DeactivateCurrentPlayset(SQLite::Database& db);
	std::string AddModToDb(SQLite::Database& db, const std::string& modName, const std::string& gameRegistryId, const std::string& dirPath);
	void AddModToPlayset(SQLite::Database& db, const std::string& modID, const std::string& playsetID);

	std::shared_ptr<Configuration> configuration;

  protected:
	wxEvtHandler* m_pParent;
};
