#include "Mod.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

Mod::Mod(const std::string& modPath)
{
	registerKeyword("name", [this](std::istream& theStream) {
		const commonItems::singleString nameString(theStream);
		name = nameString.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	parseFile(modPath);
	clearRegisteredKeywords();
	fileName = trimPath(modPath);
}
