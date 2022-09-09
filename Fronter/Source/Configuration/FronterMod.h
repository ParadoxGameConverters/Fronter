#ifndef FRONTERMOD_H
#define FRONTERMOD_H

#include "Parser.h"

class FronterMod: commonItems::parser
{
  public:
	explicit FronterMod(const std::string& modPath);
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getFileName() const { return fileName; }

  private:
	std::string name;
	std::string fileName;
};

#endif // FRONTERMOD_H