#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class Option;
class OptionBox: public wxWindow
{
  public:
	OptionBox(wxWindow* parent, const std::string& theName, std::shared_ptr<Option> theOption);

	[[nodiscard]] const auto& getOptionName() const { return optionName; }

	void loadOption(std::shared_ptr<Option> theOption) { option = theOption; }
	void initializeOption();
	
  private:
	std::string optionName;
	std::shared_ptr<Option> option;
	wxTextCtrl* textField = nullptr;
};
