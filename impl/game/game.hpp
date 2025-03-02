
#include <Windows.h>
#include <iostream>

#include "../../dependencies/framework/imgui.h"
#include "../../dependencies/framework/backend/imgui_impl_dx11.h"
#include "../../dependencies/framework/backend/imgui_impl_win32.h"

#include "../../dependencies/protection/lazyimporter.hpp"
#include "../../dependencies/protection/skcrypt.hpp"

#include "ue4.hpp"
#include "update.hpp"


inline ImColor visible_color;
inline std::string key_string = (std::string)skCrypt("Right Click");
inline ImColor Visible(150, 255, 150);
inline ImColor Invisible(255, 100, 100);

class c_game {
public:

	auto is_visible( std::uintptr_t skeletal_mesh ) -> bool;

	auto decryptname( std::uintptr_t playerstate ) -> std::string;

	auto getbone( std::uintptr_t skeletalmesh, int index ) -> fvector;

	auto worldtoscreen( fvector worldlocation ) -> fvector2d;

	auto updatecamera( ) -> void;

	auto cachedata( ) -> void;

	auto actorloop( ) -> void;

}; static c_game* game = new c_game();