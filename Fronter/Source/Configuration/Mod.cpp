#include "Mod.h"
#include "Log.h"
#include "ParserHelpers.h"

Mod::Mod(const std::string& modPath)
{
	registerKeyword("name", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString nameString(theStream);
		name = nameString.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseFile(modPath);
	clearRegisteredKeywords();
}
