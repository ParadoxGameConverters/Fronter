#ifndef VERSION_PARSER_H
#define VERSION_PARSER_H

#include "Parser.h"

class VersionParser: commonItems::parser
{
  public:
	explicit VersionParser(const std::string& thePath);
	[[nodiscard]] const auto& getVersion() const { return version; }

  private:
	void registerKeys();

	std::string version;
};

#endif // VERSION_PARSER_H