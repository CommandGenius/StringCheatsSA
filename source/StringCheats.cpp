#include "plugin.h"
#include "CCheat.h"
#include "commonshit.h"
#include "stdint.h"
#include "CCutsceneMgr.h"
#include "CHud.h"
#include "CText.h"
#include "CStats.h"
#include "eCheats.h"

using namespace plugin;

struct Cheat {
	DWORD   installAddress;
	void* method;
	const   std::string methodName;
	uint32  hash;
	eCheats type;
};

static void Toggle(eCheats cheat) { CCheat::m_aCheatsActive[cheat] ^= true; }

const auto cheats = std::to_array<Cheat>({
		{ 0x4385b0,  CCheat::WeaponCheat1, "WeaponCheat1", 0xde4b237d, CHEAT_WEAPON_SET1 },
		{ 0x438890,  CCheat::WeaponCheat2, "WeaponCheat2", 0xb22a28d1, CHEAT_WEAPON_SET2 },
		{ 0x438b30,  CCheat::WeaponCheat3, "WeaponCheat3", 0x5a783fae, CHEAT_WEAPON_SET3 },
		{ 0x438e40,  CCheat::MoneyArmourHealthCheat, "MoneyArmourHealthCheat", 0xeeccea2b, CHEAT_HEALTH_ARMOR_250K },
		{ 0x438e90,  CCheat::WantedLevelUpCheat, "WantedLevelUpCheat", 0x42af1e28, CHEAT_WANTED_LEVEL_2STARS },
		{ 0x438f20,  CCheat::WantedLevelDownCheat, "WantedLevelDownCheat", 0x555fc201, CHEAT_CLEAR_WANTED_LEVEL },
		{ 0x438f40,  CCheat::SunnyWeatherCheat, "SunnyWeatherCheat", 0x2a845345, CHEAT_SUNNY_WEATHER },
		{ 0x438f50,  CCheat::ExtraSunnyWeatherCheat, "ExtraSunnyWeatherCheat", 0xe1ef01ea, CHEAT_VERY_SUNNY_WEATHER },
		{ 0x438f60,  CCheat::CloudyWeatherCheat, "CloudyWeatherCheat", 0x771b83fc, CHEAT_OVERCAST_WEATHER },
		{ 0x438f70,  CCheat::RainyWeatherCheat, "RainyWeatherCheat", 0x5bf12848, CHEAT_RAINY_WEATHER },
		{ 0x438f80,  CCheat::FoggyWeatherCheat, "FoggyWeatherCheat", 0x44453a17, CHEAT_FOGGY_WEATHER },
		{ 0x0, nullptr, "", 0xfcff1d08, CHEAT_FASTER_CLOCK },
		{ 0x438f90,  CCheat::FastTimeCheat, "FastTimeCheat", 0xb69e8532, CHEAT_FASTER_GAMEPLAY },
		{ 0x438fc0,  CCheat::SlowTimeCheat, "SlowTimeCheat", 0x8b828076, CHEAT_SLOWER_GAMEPLAY },
		{ 0x439b20,  CCheat::MayhemCheat, "MayhemCheat", 0xdd6ed9e9, CHEAT_PEDS_ATTACK_OTHER_WITH_GOLFCLUB },
		{ 0x439c70,  CCheat::EverybodyAttacksPlayerCheat, "EverybodyAttacksPlayerCheat", 0xa290fd8c, CHEAT_HAVE_ABOUNTY_ON_YOUR_HEAD },
		{ 0x0, nullptr, "", 0x3484b5a7, CHEAT_EVERYONE_ARMED },
		{ 0x43a4a0,  CCheat::TankCheat, "TankCheat", 0x43db914e, CHEAT_SPAWN_RHINO },
		{ 0x43a4b0,  CCheat::StockCarCheat, "StockCarCheat", 0xdbc0dd65, CHEAT_SPAWN_BLOODRINGBANGER },
		{ 0x43a4c0,  CCheat::StockCar2Cheat, "StockCar2Cheat", 0x00000000, CHEAT_SPAWN_RANCHER },
		{ 0x43a4d0,  CCheat::StockCar3Cheat, "StockCar3Cheat", 0xd08a30fe, CHEAT_SPAWN_HOTRINGA },
		{ 0x43a4e0,  CCheat::StockCar4Cheat, "StockCar4Cheat", 0x37bf1b4e, CHEAT_SPAWN_HOTRINGB },
		{ 0x43a4f0,  CCheat::HearseCheat, "HearseCheat", 0xb5d40866, CHEAT_SPAWN_ROMERO },
		{ 0x43a500,  CCheat::LovefistCheat, "LovefistCheat", 0xe63b0d99, CHEAT_SPAWN_STRETCH },
		{ 0x43a510,  CCheat::TrashmasterCheat, "TrashmasterCheat", 0x675b8945, CHEAT_SPAWN_TRASHMASTER },
		{ 0x43a520,  CCheat::GolfcartCheat, "GolfcartCheat", 0x4987d5ee, CHEAT_SPAWN_CADDY },
		{ 0x439d80,  CCheat::BlowUpCarsCheat, "BlowUpCarsCheat", 0x2e8f84e8, CHEAT_BLOW_UP_ALL_CARS },
		{ 0x0, nullptr, "", 0x1a9aa3d6, CHEAT_INVISIBLE_CAR },
		{ 0x0, nullptr, "", 0xe842f3bc, CHEAT_PERFECT_HANDLING },
		{ 0x438ff0,  CCheat::SuicideCheat, "SuicideCheat", 0x0d5c6a4e, CHEAT_SUICIDE },
		{ 0x0, nullptr, "", 0x74d4fcb1, CHEAT_BGREEN_LIGHTS_CHEAT },
		{ 0x0, nullptr, "", CHEAT_AGGRESSIVE_DRIVERS },
		{ 0x4390d0,  CCheat::PinkCarsCheat, "PinkCarsCheat", 0x66516ebc, CHEAT_PINK_TRAFFIC },
		{ 0x4390f0,  CCheat::BlackCarsCheat, "BlackCarsCheat", 0x4b137e45, CHEAT_BLACK_TRAFFIC },
		{ 0x0, nullptr, "", 0x00000000, CHEAT_CARS_ON_WATER },
		{ 0x0, nullptr, "", 0x78520e33, CHEAT_BOATS_FLY },
		{ 0x439110,  CCheat::FatCheat, "FatCheat", 0x3a577325, CHEAT_FAT_PLAYER },
		{ 0x439150,  CCheat::MuscleCheat, "MuscleCheat", 0xd4966d59, CHEAT_MAX_MUSCLE },
		{ 0x439190,  CCheat::SkinnyCheat, "SkinnyCheat", 0x5fd1b49d, CHEAT_SKINNY_PLAYER },
		{ 0x4391d0,  CCheat::ElvisLivesCheat, "ElvisLivesCheat", 0xa7613f99, CHEAT_ELVIS_IS_EVERYWHERE },
		{ 0x439dd0,  CCheat::VillagePeopleCheat, "VillagePeopleCheat", 0x1792d871, CHEAT_PEDS_ATTACK_YOU_WITH_ROCKETS },
		{ 0x439230,  CCheat::BeachPartyCheat, "BeachPartyCheat", 0xcbc579df, CHEAT_BEACH_PARTY },
		{ 0x439360,  CCheat::GangsCheat, "GangsCheat", 0x4fedccff, CHEAT_GANGMEMBERS_EVERYWHERE },
		{ 0x4393d0,  CCheat::GangLandCheat, "GangLandCheat", CHEAT_GANGS_CONTROLS_THE_STREETS },
		{ 0x439e50,  CCheat::NinjaCheat, "NinjaCheat", 0x44b34866, CHEAT_NINJA_THEME },
		{ 0x4393f0,  CCheat::LoveConquersAllCheat, "LoveConquersAllCheat", 0x2ef877db, CHEAT_SLUT_MAGNET },
		{ 0x4394b0,  CCheat::AllCarsAreShitCheat, "AllCarsAreShitCheat", 0x2781e797, CHEAT_CHEAP_TRAFFIC },
		{ 0x4394e0,  CCheat::AllCarsAreGreatCheat, "AllCarsAreGreatCheat", 0x2bc1a045, CHEAT_FAST_TRAFFIC },
		{ 0x0, nullptr, "", 0xb2afe368, CHEAT_CARS_FLY },
		{ 0x0, nullptr, "", 0xfa8dd45b, CHEAT_HUGE_BUNNY_HOP },
		{ 0x43a530,  CCheat::FlyboyCheat, "FlyboyCheat", 0x8ded75bd, CHEAT_SPAWN_HYDRA },
		{ 0x43a540,  CCheat::VortexCheat, "VortexCheat", 0x1a5526bc, CHEAT_SPAWN_VORTEX },
		{ 0x0, nullptr, "", 0xa48a770b, CHEAT_SMASH_N_BOOM },
		{ 0x0, nullptr, "", 0xb07d3b32, CHEAT_ALL_CARS_HAVE_NITRO },
		{ 0x0, nullptr, "", 0x80c1e54b, CHEAT_CARS_FLOAT_AWAY_WHEN_HIT },
		{ 0x439510,  CCheat::MidnightCheat, "MidnightCheat", CHEAT_ALWAYS_MIDNIGHT },
		{ 0x439540,  CCheat::DuskCheat, "DuskCheat", 0x5dad0087, CHEAT_STOP_GAME_CLOCK_ORANGE_SKY },
		{ 0x439570,  CCheat::StormCheat, "StormCheat", 0x7f80b950, CHEAT_THUNDER_STORM },
		{ 0x439590,  CCheat::SandstormCheat, "SandstormCheat", 0x6c0fa650, CHEAT_SAND_STORM },
		{ 0x4395a0,  CCheat::PredatorCheat, "PredatorCheat", 0xf46f2fa4, CHEAT_UNUSED },
		{ 0x0, nullptr, "", 0x70164385, CHEAT_MEGAJUMP },
		{ 0x0, nullptr, "", 0x00000000, CHEAT_INFINITE_HEALTH },
		{ 0x0, nullptr, "", 0x885d0b50, CHEAT_INFINITE_OXYGEN },
		{ 0x4395b0,  CCheat::ParachuteCheat, "ParachuteCheat", 0x151bdcb3, CHEAT_GET_PARACHUTE },
		{ 0x439600,  CCheat::JetpackCheat, "JetpackCheat", 0xadfa640a, CHEAT_GET_JETPACK },
		{ 0x4396c0,  CCheat::NotWantedCheat, "NotWantedCheat", 0xe57f96ce, CHEAT_I_DO_AS_I_PLEASE },
		{ 0x4396f0,  CCheat::WantedCheat, "WantedCheat", 0x040cf761, CHEAT_SIX_WANTED_STARS },
		{ 0x0, nullptr, "", 0xe1b33eb9, CHEAT_MEGA_PUNCH },
		{ 0x0, nullptr, "", 0xfeda77f7, CHEAT_NEVER_GET_HUNGRY },
		{ 0x439710,  CCheat::RiotCheat, "RiotCheat", 0x8ca870dd, CHEAT_RIOT_MODE },
		{ 0x439720,  CCheat::FunhouseCheat, "FunhouseCheat", 0x9a629401, CHEAT_FUNHOUSE_THEME },
		{ 0x439880,  CCheat::AdrenalineCheat, "AdrenalineCheat", 0xf53ef5a5, CHEAT_ADRENALINE_MODE },
		{ 0x0, nullptr, "", 0xf2aa0c1d, CHEAT_INFINITE_AMMO },
		{ 0x4398d0,  CCheat::DrivebyCheat, "DrivebyCheat", 0xf36345a8, CHEAT_WEAPON_AIMING_WHILE_DRIVING },
		{ 0x0, nullptr, "", 0x8990d5e1, CHEAT_REDUCED_TRAFFIC },
		{ 0x439f60,  CCheat::CountrysideInvasionCheat, "CountrysideInvasionCheat", 0xb7013b1b, CHEAT_COUNTRY_TRAFFIC },
		{ 0x0, nullptr, "", 0xcaec94ee, CHEAT_WANNA_BE_IN_MY_GANG },
		{ 0x0, nullptr, "", 0x31f0c3cc, CHEAT_NO_ONE_CAN_STOP_US },
		{ 0x0, nullptr, "", 0xb3b3e72a, CHEAT_ROCKET_MAYHEM },
		{ 0x0, nullptr, "", 0xc25cdbff, CHEAT_MAX_RESPECT },
		{ 0x0, nullptr, "", 0xd5cf4eff, CHEAT_MAX_SEX_APPEAL },
		{ 0x439930,  CCheat::StaminaCheat, "StaminaCheat", 0x680416b1, CHEAT_MAX_STAMINA },
		{ 0x439940,  CCheat::WeaponSkillsCheat, "WeaponSkillsCheat", 0xcf5fda18, CHEAT_HITMAN_LEVEL_FOR_ALL_WEAPONS },
		{ 0x4399d0,  CCheat::VehicleSkillsCheat, "VehicleSkillsCheat", 0xf01286e9, CHEAT_MAX_DRIVING_SKILLS },
		{ 0x43a550,  CCheat::ApacheCheat, "ApacheCheat", 0xa841cc0a, CHEAT_SPAWN_HUNTER },
		{ 0x43a560,  CCheat::QuadCheat, "QuadCheat", 0x31ea09cf, CHEAT_SPAWN_QUAD },
		{ 0x43a570,  CCheat::TankerCheat, "TankerCheat", 0xe958788a, CHEAT_SPAWN_TANKER_TRUCK },
		{ 0x43a660,  CCheat::DozerCheat, "DozerCheat", 0x02c83a7c, CHEAT_SPAWN_DOZER },
		{ 0x43a670,  CCheat::StuntPlaneCheat, "StuntPlaneCheat", 0xe49c3ed4, CHEAT_SPAWN_STUNT_PLANE },
		{ 0x43a680,  CCheat::MonsterTruckCheat, "MonsterTruckCheat", 0x171ba8cc, CHEAT_SPAWN_MONSTER },
		{ 0x0, nullptr, "", 0x86988dae, CHEAT_PROSTITUTES_PAY_YOU },
		{ 0x0, nullptr, "", 0x2bdd2fa1, CHEAT_ALL_TAXIS_NITRO },
	});
class StringCheats {
public:
	static void AddToPCCheatString(char c)
	{
		if (CCutsceneMgr::ms_running) {
			return;
		}

		for (int32 i = ARRAY_SIZE(CCheat::m_CheatString) - 2; i >= 0; i--)
			CCheat::m_CheatString[i + 1] = CCheat::m_CheatString[i];

		CCheat::m_CheatString[0] = c;

#define _CHEATCMP(str) strncmp(str, CCheat::m_CheatString, sizeof(str)-1)

		// "THUGSARMOURY"
		if (!_CHEATCMP("YRUOMRASGUHT")) {
			if (CCheat::m_aCheatsActive[CHEAT_WEAPON_SET1]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::WeaponCheat1();
		}

		// "PROFESSIONALSKIT"
		if (!_CHEATCMP("TIKSLANOISSEFORP")) {
			if (CCheat::m_aCheatsActive[CHEAT_WEAPON_SET2]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::WeaponCheat2();
		}

		// "NUTTERSTOYS"
		if (!_CHEATCMP("SYOTSRETTUN")) {
			if (CCheat::m_aCheatsActive[CHEAT_WEAPON_SET3]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::WeaponCheat3();
		}

		// "INEEDSOMEHELP"
		if (!_CHEATCMP("PLEHEMOSDEENI")) {
			if (CCheat::m_aCheatsActive[CHEAT_HEALTH_ARMOR_250K]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::MoneyArmourHealthCheat();
		}

		// "TURNUPTHEHEAT"
		if (!_CHEATCMP("TAEHEHTPUNRUT")) {
			if (CCheat::m_aCheatsActive[CHEAT_WANTED_LEVEL_2STARS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::WantedLevelUpCheat();
		}

		// "TURNDOWNTHEHEAT"
		if (!_CHEATCMP("TAEHEHTNWODNRUT")) {
			if (CCheat::m_aCheatsActive[CHEAT_CLEAR_WANTED_LEVEL]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::WantedLevelDownCheat();
		}

		// "PLEASANTLYWARM"
		if (!_CHEATCMP("MRAWYLTNASAELP")) {
			if (CCheat::m_aCheatsActive[CHEAT_SUNNY_WEATHER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::SunnyWeatherCheat();
		}

		// "TOODAMNHOT"
		if (!_CHEATCMP("TOHNMADOOT")) {
			if (CCheat::m_aCheatsActive[CHEAT_VERY_SUNNY_WEATHER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::ExtraSunnyWeatherCheat();
		}

		// "DULLDULLDAY"
		if (!_CHEATCMP("YADLLUDLLUD")) {
			if (CCheat::m_aCheatsActive[CHEAT_OVERCAST_WEATHER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::CloudyWeatherCheat();
		}

		// "STAYINANDWATCHTV"
		if (!_CHEATCMP("VTHCTAWDNANIYATS")) {
			if (CCheat::m_aCheatsActive[CHEAT_RAINY_WEATHER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::RainyWeatherCheat();
		}

		// "CANTSEEWHEREIMGOING"
		if (!_CHEATCMP("GNIOGMIEREHWEESTNAC")) {
			if (CCheat::m_aCheatsActive[CHEAT_FOGGY_WEATHER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::FoggyWeatherCheat();
		}

		// "TIMEJUSTFLIESBY"
		if (!_CHEATCMP("YBSEILFTSUJEMIT")) {
			if (CCheat::m_aCheatsActive[CHEAT_FASTER_CLOCK]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_FASTER_CLOCK);
		}

		// "SPEEDITUP"
		if (!_CHEATCMP("PUTIDEEPS")) {
			if (CCheat::m_aCheatsActive[CHEAT_FASTER_GAMEPLAY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::FastTimeCheat();
		}

		// "SLOWITDOWN"
		if (!_CHEATCMP("NWODTIWOLS")) {
			if (CCheat::m_aCheatsActive[CHEAT_SLOWER_GAMEPLAY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::SlowTimeCheat();
		}

		// "ROUGHNEIGHBOURHOOD"
		if (!_CHEATCMP("DOOHRUOBHGIENHGUOR")) {
			if (CCheat::m_aCheatsActive[CHEAT_PEDS_ATTACK_OTHER_WITH_GOLFCLUB]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::MayhemCheat();
		}

		// "STOPPICKINGONME"
		if (!_CHEATCMP("EMNOGNIKCIPPOTS")) {
			if (CCheat::m_aCheatsActive[CHEAT_HAVE_ABOUNTY_ON_YOUR_HEAD]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::EverybodyAttacksPlayerCheat();
		}

		// "SURROUNDEDBYNUTTERS"
		if (!_CHEATCMP("SRETTUNYBDEDNUORRUS")) {
			if (CCheat::m_aCheatsActive[CHEAT_EVERYONE_ARMED]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_EVERYONE_ARMED);
		}

		// "TIMETOKICKASS"
		if (!_CHEATCMP("SSAKCIKOTEMIT")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_RHINO]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::TankCheat();
		}

		// "OLDSPEEDDEMON"
		if (!_CHEATCMP("NOMEDDEEPSDLO")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_BLOODRINGBANGER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::StockCarCheat();
		}

		// "DOUGHNUTHANDICAP"
		if (!_CHEATCMP("PACIDNAHTUNHGUOD")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_RANCHER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::StockCar2Cheat();
		}

		// "NOTFORPUBLICROADS"
		if (!_CHEATCMP("SDAORCILBUPROFTON")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_HOTRINGA]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::StockCar3Cheat();
		}

		// "JUSTTRYANDSTOPME"
		if (!_CHEATCMP("EMPOTSDNAYRTTSUJ")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_HOTRINGB]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::StockCar4Cheat();
		}

		// "WHERESTHEFUNERAL"
		if (!_CHEATCMP("LARENUFEHTSEREHW")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_ROMERO]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::HearseCheat();
		}

		// "CELEBRITYSTATUS"
		if (!_CHEATCMP("SUTATSYTIRBELEC")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_STRETCH]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::LovefistCheat();
		}

		// "TRUEGRIME"
		if (!_CHEATCMP("EMIRGEURT")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_TRASHMASTER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::TrashmasterCheat();
		}

		// "18HOLES"
		if (!_CHEATCMP("SELOH81")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_CADDY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::GolfcartCheat();
		}

		// "ALLCARSGOBOOM"
		if (!_CHEATCMP("MOOBOGSRACLLA")) {
			if (CCheat::m_aCheatsActive[CHEAT_BLOW_UP_ALL_CARS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::BlowUpCarsCheat();
		}

		// "WHEELSONLYPLEASE"
		if (!_CHEATCMP("ESAELPYLNOSLEEHW")) {
			if (CCheat::m_aCheatsActive[CHEAT_INVISIBLE_CAR]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_INVISIBLE_CAR);
		}

		// "STICKLIKEGLUE"
		if (!_CHEATCMP("EULGEKILKCITS")) {
			if (CCheat::m_aCheatsActive[CHEAT_PERFECT_HANDLING]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_PERFECT_HANDLING);
		}

		// "GOODBYECRUELWORLD"
		if (!_CHEATCMP("DLROWLEURCEYBDOOG")) {
			if (CCheat::m_aCheatsActive[CHEAT_SUICIDE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::SuicideCheat();
		}

		// "DONTTRYANDSTOPME"
		if (!_CHEATCMP("EMPOTSDNAYRTTNOD")) {
			if (CCheat::m_aCheatsActive[CHEAT_BGREEN_LIGHTS_CHEAT]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_BGREEN_LIGHTS_CHEAT);
		}

		// "ALLDRIVERSARECRIMINALS"
		if (!_CHEATCMP("SLANIMIRCERASREVIRDLLA")) {
			if (CCheat::m_aCheatsActive[CHEAT_AGGRESSIVE_DRIVERS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_AGGRESSIVE_DRIVERS);
		}

		// "PINKISTHENEWCOOL"
		if (!_CHEATCMP("LOOCWENEHTSIKNIP")) {
			if (CCheat::m_aCheatsActive[CHEAT_PINK_TRAFFIC]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::PinkCarsCheat();
		}

		// "SOLONGASITSBLACK"
		if (!_CHEATCMP("KCALBSTISAGNOLOS")) {
			if (CCheat::m_aCheatsActive[CHEAT_BLACK_TRAFFIC]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::BlackCarsCheat();
		}

		// "SEAROADER"
		if (!_CHEATCMP("REDAORAES")) {
			if (CCheat::m_aCheatsActive[CHEAT_CARS_ON_WATER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_CARS_ON_WATER);
		}

		// "FLYINGFISH"
		if (!_CHEATCMP("HSIFGNIYLF")) {
			if (CCheat::m_aCheatsActive[CHEAT_BOATS_FLY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_BOATS_FLY);
		}

		// "WHOATEALLTHEPIES"
		if (!_CHEATCMP("SEIPEHTLLAETAOHW")) {
			if (CCheat::m_aCheatsActive[CHEAT_FAT_PLAYER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::FatCheat();
		}

		// "BUFFMEUP"
		if (!_CHEATCMP("PUEMFFUB")) {
			if (CCheat::m_aCheatsActive[CHEAT_MAX_MUSCLE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::MuscleCheat();
		}

		// "LEANANDMEAN"
		if (!_CHEATCMP("NAEMDNANAEL")) {
			if (CCheat::m_aCheatsActive[CHEAT_MAX_MUSCLE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::SkinnyCheat();
		}

		// "BLUESUEDESHOES"
		if (!_CHEATCMP("SEOHSEDEUSEULB")) {
			if (CCheat::m_aCheatsActive[CHEAT_ELVIS_IS_EVERYWHERE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::ElvisLivesCheat();
		}

		// "ATTACKOFTHEVILLAGEPEOPLE"
		if (!_CHEATCMP("ELPOEPEGALLIVEHTFOKCATTA")) {
			if (CCheat::m_aCheatsActive[CHEAT_PEDS_ATTACK_YOU_WITH_ROCKETS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::VillagePeopleCheat();
		}

		// "LIFESABEACH"
		if (!_CHEATCMP("HCAEBASEFIL")) {
			if (CCheat::m_aCheatsActive[CHEAT_BEACH_PARTY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::BeachPartyCheat();
		}

		// "ONLYHOMIESALLOWED"
		if (!_CHEATCMP("DEWOLLASEIMOHYLNO")) {
			if (CCheat::m_aCheatsActive[CHEAT_GANGMEMBERS_EVERYWHERE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::GangsCheat();
		}

		// "BETTERSTAYINDOORS"
		if (!_CHEATCMP("SROODNIYATSRETTEB")) {
			if (CCheat::m_aCheatsActive[CHEAT_GANGS_CONTROLS_THE_STREETS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::GangLandCheat();
		}

		// "NINJATOWN"
		if (!_CHEATCMP("NWOTAJNIN")) {
			if (CCheat::m_aCheatsActive[CHEAT_NINJA_THEME]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::NinjaCheat();
		}

		// "LOVECONQUERSALL"
		if (!_CHEATCMP("LLASREUQNOCEVOL")) {
			if (CCheat::m_aCheatsActive[CHEAT_SLUT_MAGNET]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::LoveConquersAllCheat();
		}

		// "EVERYONEISPOOR"
		if (!_CHEATCMP("ROOPSIENOYREVE")) {
			if (CCheat::m_aCheatsActive[CHEAT_CHEAP_TRAFFIC]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::AllCarsAreShitCheat();
		}

		// "EVERYONEISRICH"
		if (!_CHEATCMP("HCIRSIENOYREVE")) {
			if (CCheat::m_aCheatsActive[CHEAT_FAST_TRAFFIC]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::AllCarsAreGreatCheat();
		}

		// "CHITTYCHITTYBANGBANG"
		if (!_CHEATCMP("GNABGNABYTTIHCYTTIHC")) {
			if (CCheat::m_aCheatsActive[CHEAT_CARS_FLY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_CARS_FLY);
		}

		// "CJPHONEHOME"
		if (!_CHEATCMP("EMOHENOHPJC")) {
			if (CCheat::m_aCheatsActive[CHEAT_HUGE_BUNNY_HOP]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_HUGE_BUNNY_HOP);
		}

		// "JUMPJET"
		if (!_CHEATCMP("TEJPMUJ")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_HYDRA]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::FlyboyCheat();
		}

		// "IWANTTOHOVER"
		if (!_CHEATCMP("REVOHOTTNAWI")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_VORTEX]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::VortexCheat();
		}

		// "TOUCHMYCARYOUDIE"
		if (!_CHEATCMP("EIDUOYRACYMHCUOT")) {
			if (CCheat::m_aCheatsActive[CHEAT_SMASH_N_BOOM]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_SMASH_N_BOOM);
		}

		// "SPEEDFREAK"
		if (!_CHEATCMP("KAERFDEEPS")) {
			if (CCheat::m_aCheatsActive[CHEAT_ALL_CARS_HAVE_NITRO]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_ALL_CARS_HAVE_NITRO);
		}

		// "BUBBLECARS"
		if (!_CHEATCMP("SRACELBBUB")) {
			if (CCheat::m_aCheatsActive[CHEAT_CARS_FLOAT_AWAY_WHEN_HIT]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_CARS_FLOAT_AWAY_WHEN_HIT);
		}

		// "NIGHTPROWLER"
		if (!_CHEATCMP("RELWORPTHGIN")) {
			if (CCheat::m_aCheatsActive[CHEAT_ALWAYS_MIDNIGHT]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::MidnightCheat();
		}

		// "DONTBRINGONTHENIGHT"
		if (!_CHEATCMP("THGINEHTNOGNIRBTNOD")) {
			if (CCheat::m_aCheatsActive[CHEAT_STOP_GAME_CLOCK_ORANGE_SKY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::DuskCheat();
		}

		// "SCOTTISHSUMMER"
		if (!_CHEATCMP("REMMUSHSITTOCS")) {
			if (CCheat::m_aCheatsActive[CHEAT_THUNDER_STORM]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::StormCheat();
		}

		// "SANDINMYEARS"
		if (!_CHEATCMP("SRAEYMNIDNAS")) {
			if (CCheat::m_aCheatsActive[CHEAT_SAND_STORM]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::SandstormCheat();
		}

		// "KANGAROO"
		if (!_CHEATCMP("OORAGNAK")) {
			if (CCheat::m_aCheatsActive[CHEAT_MEGAJUMP]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_MEGAJUMP);
		}

		// "NOONECANHURTME"
		if (!_CHEATCMP("EMTRUHNACENOON")) {
			if (CCheat::m_aCheatsActive[CHEAT_INFINITE_HEALTH]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_INFINITE_HEALTH);
		}

		// "MANFROMATLANTIS"
		if (!_CHEATCMP("SITNALTAMORFNAM")) {
			if (CCheat::m_aCheatsActive[CHEAT_INFINITE_OXYGEN]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_INFINITE_OXYGEN);
		}

		// "LETSGOBASEJUMPING"
		if (!_CHEATCMP("GNIPMUJESABOGSTEL")) {
			if (CCheat::m_aCheatsActive[CHEAT_GET_PARACHUTE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::ParachuteCheat();
		}

		// "ROCKETMAN"
		if (!_CHEATCMP("NAMTEKCOR")) {
			if (CCheat::m_aCheatsActive[CHEAT_GET_JETPACK]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::JetpackCheat();
		}

		// "IDOASIPLEASE"
		if (!_CHEATCMP("ESAELPISAODI")) {
			if (CCheat::m_aCheatsActive[CHEAT_I_DO_AS_I_PLEASE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::NotWantedCheat();
		}

		// "BRINGITON"
		if (!_CHEATCMP("NOTIGNIRB")) {
			if (CCheat::m_aCheatsActive[CHEAT_SIX_WANTED_STARS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::WantedCheat();
		}

		// "STINGLIKEABEE"
		if (!_CHEATCMP("EEBAEKILGNITS")) {
			if (CCheat::m_aCheatsActive[CHEAT_MEGA_PUNCH]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_MEGA_PUNCH);
		}

		// "IAMNEVERHUNGRY"
		if (!_CHEATCMP("YRGNUHREVENMAI")) {
			if (CCheat::m_aCheatsActive[CHEAT_NEVER_GET_HUNGRY]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_NEVER_GET_HUNGRY);
		}

		// "STATEOFEMERGENCY"
		if (!_CHEATCMP("YCNEGREMEFOETATS")) {
			if (CCheat::m_aCheatsActive[CHEAT_RIOT_MODE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::RiotCheat();
		}

		// "CRAZYTOWN"
		if (!_CHEATCMP("NWOTYZARC")) {
			if (CCheat::m_aCheatsActive[CHEAT_FUNHOUSE_THEME]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::FunhouseCheat();
		}

		// "TAKEACHILLPILL"
		if (!_CHEATCMP("LLIPLLIHCAEKAT")) {
			if (CCheat::m_aCheatsActive[CHEAT_ADRENALINE_MODE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::AdrenalineCheat();
		}

		// "FULLCLIP"
		if (!_CHEATCMP("PILCLLUF")) {
			if (CCheat::m_aCheatsActive[CHEAT_INFINITE_AMMO]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_INFINITE_AMMO);
		}

		// "IWANNADRIVEBY"
		if (!_CHEATCMP("YBEVIRDANNAWI")) {
			if (CCheat::m_aCheatsActive[CHEAT_WEAPON_AIMING_WHILE_DRIVING]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::DrivebyCheat();
		}

		// "GHOSTTOWN"
		if (!_CHEATCMP("NWOTTSOHG")) {
			if (CCheat::m_aCheatsActive[CHEAT_REDUCED_TRAFFIC]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_REDUCED_TRAFFIC);
		}

		// "HICKSVILLE"
		if (!_CHEATCMP("ELLIVSKCIH")) {
			if (CCheat::m_aCheatsActive[CHEAT_COUNTRY_TRAFFIC]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::CountrysideInvasionCheat();
		}

		// "WANNABEINMYGANG"
		if (!_CHEATCMP("GNAGYMNIEBANNAW")) {
			if (CCheat::m_aCheatsActive[CHEAT_WANNA_BE_IN_MY_GANG]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_WANNA_BE_IN_MY_GANG);
		}

		// "NOONECANSTOPUS"
		if (!_CHEATCMP("SUPOTSNACENOON")) {
			if (CCheat::m_aCheatsActive[CHEAT_NO_ONE_CAN_STOP_US]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_NO_ONE_CAN_STOP_US);
		}

		// "ROCKETMAYHEM"
		if (!_CHEATCMP("MEHYAMTEKCOR")) {
			if (CCheat::m_aCheatsActive[CHEAT_ROCKET_MAYHEM]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_ROCKET_MAYHEM);
		}

		// "WORSHIPME"
		if (!_CHEATCMP("EMPIHSROW")) {
			if (CCheat::m_aCheatsActive[CHEAT_MAX_RESPECT]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_MAX_RESPECT);
		}

		// "HELLOLADIES"
		if (!_CHEATCMP("SEIDALOLLEH")) {
			if (CCheat::m_aCheatsActive[CHEAT_MAX_SEX_APPEAL]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			ApplyCheat(CHEAT_MAX_SEX_APPEAL);
		}

		// "ICANGOALLNIGHT"
		if (!_CHEATCMP("THGINLLAOGNACI")) {
			if (CCheat::m_aCheatsActive[CHEAT_MAX_STAMINA]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::StaminaCheat();
		}

		// "PROFESSIONALKILLER"
		if (!_CHEATCMP("RELLIKLANOISSEFORP")) {
			if (CCheat::m_aCheatsActive[CHEAT_HITMAN_LEVEL_FOR_ALL_WEAPONS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::WeaponSkillsCheat();
		}

		// "NATURALTALENT"
		if (!_CHEATCMP("TNELATLARUTAN")) {
			if (CCheat::m_aCheatsActive[CHEAT_MAX_DRIVING_SKILLS]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::VehicleSkillsCheat();
		}

		// "OHDUDE"
		if (!_CHEATCMP("EDUDHO")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_HUNTER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::ApacheCheat();
		}

		// "FOURWHEELFUN"
		if (!_CHEATCMP("NUFLEEHWRUOF")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_QUAD]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::QuadCheat();
		}

		// "HITTHEROADJACK"
		if (!_CHEATCMP("KCAJDAOREHTTIH")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_TANKER_TRUCK]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::TankerCheat();
		}

		// "ITSALLBULL"
		if (!_CHEATCMP("LLUBLLASTI")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_DOZER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::DozerCheat();
		}

		// "FLYINGTOSTUNT"
		if (!_CHEATCMP("TNUTSOTGNIYLF")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_STUNT_PLANE]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::StuntPlaneCheat();
		}

		// "MONSTERMASH"
		if (!_CHEATCMP("HSAMRETSNOM")) {
			if (CCheat::m_aCheatsActive[CHEAT_SPAWN_MONSTER]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;
			CCheat::MonsterTruckCheat();
		}
#undef _CHEATCMP
	}
	static void ApplyCheat(eCheats cheat) {
		auto func = CCheat::m_aCheatFunctions[cheat];
		if (func) {
			return func();
		}
		else {
			return Toggle(cheat);
		}
	}

	StringCheats() {
		patch::RedirectJump(0x438480, AddToPCCheatString);
		patch::RedirectJump(0x438370, ApplyCheat);
		for (auto& cheat : cheats) {
			if (cheat.installAddress == 0x0) {
				continue;
			}

			for (auto& cheatFunc : CCheat::m_aCheatFunctions) {
				if (reinterpret_cast<unsigned long>(cheatFunc) == cheat.installAddress) {
					cheatFunc = static_cast<void (*)()>(cheat.method);
				}
			}
		}
	}
} StringCheats;

/*			if (CCheat::m_aCheatsActive[eCheats]) {
				// deactivated
				CHud::SetHelpMessage(TheText.Get("CHEAT8"), true, false, false);
			}
			else {
				// activated
				CHud::SetHelpMessage(TheText.Get("CHEAT1"), true, false, false);
			}
			CStats::IncrementStat(STAT_TIMES_CHEATED, 1.0f);
			CCheat::m_bHasPlayerCheated = true;*/