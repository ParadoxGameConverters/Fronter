#pragma once
#include <cstdlib>
#include <string>
#include <wx/colour.h>
#include <wx/settings.h>

namespace Theme
{
inline bool IsDarkMode()
{
#ifdef __WXMSW__
	// Dark mode disabled on Windows because the native controls do not fully support it.
	return false;
#else
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
#endif
}

inline wxColour GetThemedColour(const wxColour& light, const wxColour& dark)
{
	return IsDarkMode() ? dark : light;
}
} // namespace Theme
