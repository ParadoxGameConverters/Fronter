#ifndef MOD_H
#define MOD_H

#include "Parser.h"

class Mod: commonItems::parser
{
  public:
	explicit Mod(const std::string& modPath);
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getFileName() const { return fileName; }

  private:
	std::string name;
	std::string fileName;
};

#endif // MOD_H