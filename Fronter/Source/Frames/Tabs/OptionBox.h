#pragma once
#include "../../Utils/Localization/Localization.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <memory>

class Option;
class OptionBox final: public wxWindow
{
  public:
	OptionBox(wxWindow* parent, const std::string& theName, const std::shared_ptr<Option>& theOption);

	[[nodiscard]] const auto& getOptionName() const { return optionName; }

	void loadOption(const std::shared_ptr<Option>& theOption) { option = theOption; }
	void initializeOption();
	void loadLocalization(std::shared_ptr<Localization> theLocalization) { localization = std::move(theLocalization); }

  private:
	std::string optionName;
	std::shared_ptr<Option> option;
	wxTextCtrl* textField = nullptr;
	std::shared_ptr<Localization> localization;
	std::vector<wxCheckBox*> checkBoxes;
};
