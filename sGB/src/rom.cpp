#include "rom.hpp"

std::vector<std::string> initRomNames() 
{
	std::vector<std::string> names(256);
	names.insert(names.begin() + ROM_ONLY, "ROM_ONLY");
	names.insert(names.begin() + ROM_MBC1, "ROM_MBC1");
	names.insert(names.begin() + ROM_MBC1_RAM, "ROM_MBC1_RAM");
	names.insert(names.begin() + ROM_MBC1_RAM_BATT, "ROM_MBC1_RAM_BATT");
	names.insert(names.begin() + ROM_MBC2, "ROM_MBC2");
	names.insert(names.begin() + ROM_MBC2_BATTERY, "ROM_MBC2_BATTERY");
	names.insert(names.begin() + ROM_RAM, "ROM_RAM");
	names.insert(names.begin() + ROM_RAM_BATTERY, "ROM_RAM_BATTERY");
	names.insert(names.begin() + ROM_MMM01, "ROM_MMM01");
	names.insert(names.begin() + ROM_MMM01_SRAM, "ROM_MMM01_SRAM");
	names.insert(names.begin() + ROM_MMM01_SRAM_BATT, "ROM_MMM01_SRAM_BATT");
	names.insert(names.begin() + ROM_MBC3_TIMER_BATT, "ROM_MBC3_TIMER_BATT");
	names.insert(names.begin() + ROM_MBC3_TIMER_RAM_BATT, "ROM_MBC3_TIMER_RAM_BATT");
	names.insert(names.begin() + ROM_MBC3, "ROM_MBC3");
	names.insert(names.begin() + ROM_MBC3_RAM, "ROM_MBC3_RAM");
	names.insert(names.begin() + ROM_MBC3_RAM_BATT, "ROM_MBC3_RAM_BATT");
	names.insert(names.begin() + ROM_MBC5, "ROM_MBC5");
	names.insert(names.begin() + ROM_MBC5_RAM, "ROM_MBC5_RAM");
	names.insert(names.begin() + ROM_MBC5_RAM_BATT, "ROM_MBC5_RAM_BATT");
	names.insert(names.begin() + ROM_MBC5_RUMBLE, "ROM_MBC5_RUMBLE");
	names.insert(names.begin() + ROM_MBC5_RUMBLE_SRAM, "ROM_MBC5_RUMBLE_SRAM");
	names.insert(names.begin() + ROM_MBC5_RUMBLE_SRAM_BATT, "ROM_MBC5_RUMBLE_SRAM_BATT");
	names.insert(names.begin() + ROM_POCKET_CAMERA, "ROM_POCKET_CAMERA");
	names.insert(names.begin() + ROM_BANDAI_TAMA5, "ROM_BANDAI_TAMA5");
	names.insert(names.begin() + ROM_HUDSON_HUC3, "ROM_HUDSON_HUC3");
	names.insert(names.begin() + ROM_HUDSON_HUC1, "ROM_HUDSON_HUC1");
	return names;
}

std::vector<std::string> romTypeNames = initRomNames();