#ifndef MOD_H
#define MOD_H

#include "Parser.h"

class Mod: commonItems::parser
{
  public:
	explicit Mod(const std::string& modPath);
	[[nodiscard]] const auto& getName() const { return name; }

  private:

	std::string name;
};

#endif // MOD_H