#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../../Configuration/Configuration.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include <wx/thread.h>

wxDECLARE_EVENT(wxEVT_COPIERDONE, wxCommandEvent);

class ModCopier final: public wxThread
{
  public:
	explicit ModCopier(wxEvtHandler* pParent): wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}
	void loadConfiguration(const std::shared_ptr<Configuration>& theConfiguration) { configuration = theConfiguration; }

  private:
	void* Entry() override;
	void createPlayset(const std::string& destModFolder, const std::string& targetName, const std::string& destModFolderPath);
	[[nodiscard]] static std::string getLastUpdatedLauncherDbPath(const std::string& gameDocsDirectory);
	void deactivateCurrentPlayset(SQLite::Database& db);
	[[nodiscard]] static std::string addModToDb(SQLite::Database& db, const std::string& modName, const std::string& gameRegistryId, const std::string& dirPath);
	void addModToPlayset(SQLite::Database& db, const std::string& modID, const std::string& playsetID);

	std::shared_ptr<Configuration> configuration;

  protected:
	wxEvtHandler* m_pParent;
};
