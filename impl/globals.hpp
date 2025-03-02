#include <cstdint>

namespace globals {
	inline int process_id; 
	inline uintptr_t image_base;

	inline int screen_width, screen_height;

	inline bool render_menu = 1;

	inline bool aimbot = 1;

	inline bool prediction = 1;

	inline bool radar = 0;

	inline bool visible = 1;

	inline bool weapon = 0;

	inline bool platform = 1;

	extern int aimbot_keyz; // Deklaration ohne Initialisierung

	inline bool rank = 0;

	inline int aimbot_smooth = 5;

	inline int aimbot_fov = 150;

	inline bool render_fov = 1;

	inline bool render_crosshair;

	inline int hitbox = 2;

	inline bool static_box = 0;

	inline bool distance = 1;
	
	inline bool snapline = 1;

	inline bool skeleton = 1;

	inline bool username = 0;

	inline int render_distance = 260;

	inline bool holding_shotgun;
	
	inline bool vault_tp = 0;

	inline bool b_head = 1, b_neck, b_chest, b_pelvis;
}