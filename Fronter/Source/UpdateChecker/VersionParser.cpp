#include "VersionParser.h"
#include "ParserHelpers.h"
#include "CommonRegexes.h"

VersionParser::VersionParser(const std::string& thePath)
{
	registerKeys();
	parseFile(thePath);
	clearRegisteredKeywords();
}

void VersionParser::registerKeys()
{
	registerKeyword("version", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleString versionStr(theStream);
		version = versionStr.getString();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
