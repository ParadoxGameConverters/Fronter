#pragma once
#include <wx/settings.h>
#include <wx/colour.h>
#include <cstdlib>
#include <string>

namespace Theme
{
inline bool IsDarkMode()
{
	try
	{
		const auto appearance = wxSystemSettings::GetAppearance();
		if (appearance.IsDark())
			return true;
		// Fallback: if appearance name contains "dark" or background is dark
		if (appearance.IsUsingDarkBackground())
			return true;
	}
	catch (...)
	{
		// If wx is not yet initialized or no display, try next fallback.
	}
	try
	{
		// Fallback via system window background luminance.
		wxColour bg = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
		if (bg.IsOk())
		{
			// Perceived luminance
			int luminance = static_cast<int>(0.299 * bg.Red() + 0.587 * bg.Green() + 0.114 * bg.Blue());
			if (luminance < 128)
				return true;
		}
	}
	catch (...)
	{
	}
	// Final fallback: check GTK_THEME env variable.
	if (const char* gtkTheme = std::getenv("GTK_THEME"))
	{
		std::string themeStr(gtkTheme);
		for (auto& c: themeStr)
			c = static_cast<char>(tolower(c));
		if (themeStr.find("dark") != std::string::npos)
			return true;
	}
	return false;
}

inline wxColour GetThemedColour(const wxColour& light, const wxColour& dark)
{
	return IsDarkMode() ? dark : light;
}
} // namespace Theme
