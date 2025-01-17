#ifndef FRONTERMOD_H
#define FRONTERMOD_H

#include <filesystem>
#include <Parser.h>


class FronterMod: commonItems::parser
{
  public:
	explicit FronterMod(const std::filesystem::path& modPath);
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getFileName() const { return fileName; }

  private:
	std::string name;
	std::filesystem::path fileName;
};

#endif // FRONTERMOD_H