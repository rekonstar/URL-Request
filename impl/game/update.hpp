#include <Windows.h>
#include <iostream>
#include <vector>

class c_cached {
public:
	std::uintptr_t
		gworld,
		player_controller,
		acknowledged_pawn,
		player_state,
		current_vehicle;
	int
		team_index,
		process_id;
	fvector
		relative_location;

	fvector
		camera_location,
		camera_rotation;
	float
		camera_fov;
		
}; static c_cached* cached = new c_cached( );

class player_info {
public:
	std::uintptr_t
		actor,
		skeletal_mesh,
		player_state;
	int
		team_index;
}; 

inline std::vector<player_info> p_list;
inline std::vector<player_info> p_list_temp;

#define OFFSET_GWORLD 0x13743D18 // updated
#define OFFSET_GINSTANCE 0x1f0 // updated
#define OFFSET_LOCALPLAYER 0x38 // updated
#define OFFSET_PLAYERCONTROLLER 0x30 // updated
#define OFFSET_ACKNOWLEDGEDPAWN 0x350 // updated
#define OFFSET_PLAYERSTATE 0x2c8 // updated
#define OFFSET_CURRENTWEAPON 0xa80 // updated
#define OFFSET_GSTATE 0x178 // updated
#define OFFSET_PLAYERARRAY 0x2c0 // updated
#define OFFSET_PAWNPRIVATE 0x320 // updated
#define OFFSET_SKELETALMESH 0x328 // updated
#define OFFSET_COMPTOWORLD 0x1e0 // updated
#define OFFSET_TEAMINDEX 0x1261 // updated
#define OFFSET_ROOTCOMPONENT 0x1B0 // updated
#define OFFSET_RELATIVELOCATION 0x138 // updated
#define OFFSET_PLATFORM 0x430 // updated
#define OFFSET_HABENAROCOMP 0x430 // updated
#define OFFSET_RANKEDPROGRESS 0x430 // updated
