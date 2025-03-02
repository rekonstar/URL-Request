#include <string>
#include <thread>
#include <chrono>

#include "game.hpp"

#include "../../dependencies/communication/communication.hpp"

#include "../mouse/mouse.hpp"

#include "../hexarray.hpp"

bool has_clicked;
std::chrono::steady_clock::time_point tb_begin;
std::chrono::steady_clock::time_point tb_end;
int tb_time_since;

auto in_screen(fvector2d bone) -> bool {
	if (bone.x > -30 && bone.x < globals::screen_width + 30 && bone.y > -30 && bone.y < globals::screen_height + 30) {
		return true;
	}
	else {
		return false;
	}
}

auto c_game::is_visible(uintptr_t skeletal_mesh) -> bool {
	auto Seconds = read<double>(cached->gworld + 0x150);
	auto LastRenderTime = read<float>(skeletal_mesh + 0x30C);
	return Seconds - LastRenderTime <= 0.06f;
}

auto c_game::decryptname( std::uintptr_t playerstate ) -> std::string {

	__int64 FString = read<__int64>(playerstate + 0xAF8);
	int Length = read<int>(FString + 16);
	uintptr_t FText = read<__int64>(FString + 8);

	__int64 v6 = Length;
	if (!v6) return std::string("BOT");

	wchar_t* NameBuffer = new wchar_t[Length];
	kernel->read_phys((PVOID)FText, NameBuffer, Length * sizeof(wchar_t));

	char v21;
	int v22;
	int i;
	int v25;
	WORD* v23;

	v21 = v6 - 1;
	if (!(DWORD)v6)
		v21 = 0;
	v22 = 0;
	v23 = (WORD*)NameBuffer;
	for (i = (v21) & 3; ; *v23++ += i & 7)
	{
		v25 = v6 - 1;
		if (!(DWORD)v6)
			v25 = 0;
		if (v22 >= v25)
			break;
		i += 3;
		++v22;
	}

	std::wstring wbuffer{ NameBuffer };

	return std::string(wbuffer.begin(), wbuffer.end());

}

auto c_game::getbone( std::uintptr_t skeletalmesh, int index ) -> fvector 
{
	uintptr_t bone_array = read<uintptr_t>(skeletalmesh + 0x5A8);
	if (bone_array == NULL) bone_array = read<uintptr_t>(skeletalmesh + 0x5A8 + 0x10);
	ftransform bone = read<ftransform>(bone_array + (index * 0x60));
	ftransform component_to_world = read<ftransform>(skeletalmesh + 0x1e0);
	D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
	return fvector(matrix._41, matrix._42, matrix._43);
}

auto c_game::worldtoscreen(fvector worldlocation) -> fvector2d 
{
	D3DMATRIX tempMatrix = Matrix(cached->camera_rotation, fvector(0, 0, 0));

	fvector vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	fvector vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	fvector vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	fvector vDelta = worldlocation - cached->camera_location;

	fvector vTransformed = fvector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f) vTransformed.z = 1.f;

	float ratio = (float)globals::screen_width / globals::screen_height;

	if (ratio < 4.0f / 3.0f)
		ratio = 4.0f / 3.0f;

	float fov = ratio / (16.0f / 9.0f) * (float)tanf(cached->camera_fov * M_PI / 360.0f);

	double ScreenX = (globals::screen_width / 2.0f) + vTransformed.x * (((globals::screen_width / 2.0f) / fov)) / vTransformed.z;
	double ScreenY = (globals::screen_height / 2.0f) - vTransformed.y * (((globals::screen_width / 2.0f) / fov)) / vTransformed.z;

	return fvector2d(ScreenX, ScreenY);
}
enum class EFortRarity : uint8_t
{
	EFortRarity__Common = 0,
	EFortRarity__Uncommon = 1,
	EFortRarity__Rare = 2,
	EFortRarity__Epic = 3,
	EFortRarity__Legendary = 4,
	EFortRarity__Mythic = 5,
	EFortRarity__Transcendent = 6,
	EFortRarity__Unattainable = 7,
	EFortRarity__NumRarityValues = 8,
};
//std::string get_weapon_name(uintptr_t pawn_private)
//{
//	std::string weapon_name;
//
//	uint64_t current_weapon = read<uint64_t>(pawn_private + offsets::current_weapon);
//	uint64_t weapon_data = read<uint64_t>(current_weapon + offsets::weapon_data);
//	uint64_t item_name = read<uint64_t>(weapon_data + 0x40);
//
//	uint64_t FData = read<uint64_t>(item_name + 0x18);
//	int FLength = read<int>(item_name + 0x20);
//
//	if (FLength > 0 && FLength < 50)
//	{
//		wchar_t* WeaponBuffer = new wchar_t[FLength];
//		kernel->read_phys((void*)FData, (PVOID)WeaponBuffer, FLength * sizeof(wchar_t));
//		std::wstring wstr_buf(WeaponBuffer);
//		weapon_name.append(std::string(wstr_buf.begin(), wstr_buf.end()));
//
//		delete[] WeaponBuffer;
//	}
//
//	return weapon_name;
//}

std::string get_rank_name(int tier)
{
	switch (tier)
	{
	case 0: return "Bronze I";
	case 1: return "Bronze II";
	case 2: return "Bronze III";
	case 3: return "Silver I";
	case 4: return "Silver II";
	case 5: return "Silver III";
	case 6: return "Gold I";
	case 7: return "Gold II";
	case 8: return "Gold III";
	case 9: return "Platinum I";
	case 10: return "Platinum II";
	case 11: return "Platinum III";
	case 12: return "Diamond I";
	case 13: return "Diamond II";
	case 14: return "Diamond III";
	case 15: return "Elite";
	case 16: return "Champion";
	case 17: return "Unreal";
	default: return "Unranked";
	}
}

ImVec4 get_rank_color(int tier)
{
	switch (tier)
	{
	case 0: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // Unranked
	case 1:
	case 2: return ImVec4(0.902f, 0.580f, 0.227f, 1.0f); // Bronze
	case 3:
	case 4:
	case 5: return ImVec4(0.843f, 0.843f, 0.843f, 1.0f); // Silver
	case 6:
	case 7:
	case 8: return ImVec4(1.0f, 0.871f, 0.0f, 1.0f); // Gold
	case 9:
	case 10:
	case 11: return ImVec4(0.0f, 0.7f, 0.7f, 1.0f);  // Platinum
	case 12:
	case 13:
	case 14: return ImVec4(0.1686f, 0.3294f, 0.8235f, 1.0f); // Diamond
	case 15: return ImVec4(0.7f, 0.7f, 0.7f, 1.0f);   // Elite
	case 16: return ImVec4(1.0f, 0.6f, 0.0f, 1.0f);   // Champion
	case 17: return ImVec4(0.6f, 0.0f, 0.6f, 1.0f);   // Unreal
	default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // Unranked
	}
}

ImColor get_loot_color(EFortRarity tier)
{
	if (tier == EFortRarity::EFortRarity__Common)
	{
		return ImColor(123, 123, 123, 255);
	}
	else if (tier == EFortRarity::EFortRarity__Uncommon)
	{
		return ImColor(58, 121, 19, 255);
	}
	else if (tier == EFortRarity::EFortRarity__Rare)
	{
		return ImColor(18, 88, 162, 255);
	}
	else if (tier == EFortRarity::EFortRarity__Epic)
	{
		return ImColor(189, 63, 250, 255);
	}
	else if (tier == EFortRarity::EFortRarity__Legendary)
	{
		return ImColor(255, 118, 5, 255);
	}
	else if (tier == EFortRarity::EFortRarity__Mythic)
	{
		return ImColor(220, 160, 30, 255);
	}
	else if (tier == EFortRarity::EFortRarity__Transcendent)
	{
		return ImColor(0, 225, 252, 255);
	}

	return ImColor(123, 123, 123, 255);
}

auto c_game::updatecamera( ) -> void {

	auto LocationPointer = read<uintptr_t>(cached->gworld + 0x120);
	auto RotationPointer = read<uintptr_t>(cached->gworld + 0x130);


	struct FNRotation
	{
		double a; //0x0000
		char pad_0008[24]; //0x0008
		double b; //0x0020
		char pad_0028[424]; //0x0028
		double c; //0x01D0
	}tpmrotation;

	tpmrotation.a = read<double>(RotationPointer);
	tpmrotation.b = read<double>(RotationPointer + 0x20);
	tpmrotation.c = read<double>(RotationPointer + 0x1d0);

	cached->camera_rotation.x = asin(tpmrotation.c) * (180.0 / M_PI);
	cached->camera_rotation.y = ((atan2(tpmrotation.a * -1, tpmrotation.b) * (180.0 / M_PI)) * -1) * -1;
	cached->camera_rotation.z = 0;
	cached->camera_location = read<fvector>(LocationPointer);
	cached->camera_fov = read<float>(cached->player_controller + 0x3AC) * 90.f;




}

auto c_game::cachedata( ) -> void {

	p_list_temp.clear( );

	cached->gworld = read<std::uintptr_t>(virtualaddy + OFFSET_GWORLD );
	if ( !cached->gworld ) return;

	auto g_instance = read<std::uintptr_t>(cached->gworld + OFFSET_GINSTANCE );

	auto local_player = read<std::uintptr_t>( read<std::uintptr_t>( g_instance + OFFSET_LOCALPLAYER ) );

	cached->player_controller = read<std::uintptr_t>( local_player + OFFSET_PLAYERCONTROLLER );
	
	cached->acknowledged_pawn = read<std::uintptr_t>( cached->player_controller + OFFSET_ACKNOWLEDGEDPAWN );

	auto player_state = read<std::uintptr_t>( cached->acknowledged_pawn + OFFSET_PLAYERSTATE );

	cached->team_index = read<int>( player_state + OFFSET_TEAMINDEX );

	auto root_component = read<std::uintptr_t>(cached->acknowledged_pawn + OFFSET_ROOTCOMPONENT);
	
	cached->relative_location = read<fvector>(root_component + OFFSET_RELATIVELOCATION);
	
	auto g_state = read<std::uintptr_t>(cached->gworld + OFFSET_GSTATE );

	auto player_array = read<std::uintptr_t>( g_state + OFFSET_PLAYERARRAY );

	auto player_array_size = read<int>( g_state + ( OFFSET_PLAYERARRAY + sizeof( std::uintptr_t ) ) );

	for ( int i = 0; i < player_array_size; ++i ) {

		auto player_state = read<std::uintptr_t>(player_array + (i * sizeof(std::uintptr_t))); if (!player_state) continue;

		auto current_actor = read<std::uintptr_t>(player_state + OFFSET_PAWNPRIVATE);
		if (!current_actor) continue; if (current_actor == cached->acknowledged_pawn) continue;

		auto skeletal_mesh = read<std::uintptr_t>(current_actor + OFFSET_SKELETALMESH); 
		if (!skeletal_mesh) continue;

		auto checkscreen = worldtoscreen(getbone(skeletal_mesh, bone::pelvis));
		if (!in_screen(checkscreen)) continue;

		auto team_index = read<int>(player_state + OFFSET_TEAMINDEX);
		if (cached->team_index == team_index) continue;

		player_info cached{ };

		cached.actor = current_actor;

		cached.player_state = player_state;

		cached.skeletal_mesh = skeletal_mesh;

		cached.team_index = team_index;

		p_list_temp.push_back( cached );
	}

	p_list.clear( );
	p_list = p_list_temp;
}


auto c_game::actorloop( ) -> void {

	auto target_entity = ( uintptr_t )NULL;
	auto target_dist = FLT_MAX;

	for ( auto& inc : p_list )
	{
		auto root_world = getbone( inc.skeletal_mesh, 0 );

		auto root_screen = worldtoscreen( root_world );

		auto head_world = getbone( inc.skeletal_mesh, 110 );

		auto head_screen = worldtoscreen( head_world );

		auto head_box = worldtoscreen( fvector( head_world.x, head_world.y, head_world.z + 20 ) );

		auto box_height = abs( head_box.y - root_screen.y );

		auto box_width = box_height * 0.45;

		auto distance = cached->relative_location.Distance( root_world ) / 100.0f;

		if ( is_visible( inc.skeletal_mesh ) ) {
			visible_color = Visible;
		}
		else {
			visible_color = Invisible;
		}

		/*if ( globals::aimbot ) {
			auto dx = head_screen.x - ( globals::screen_width / 2 );
			auto dy = head_screen.y - ( globals::screen_height / 2 );
			auto dist = sqrtf( dx * dx + dy * dy );

			if ( is_visible( inc.skeletal_mesh ) ) {
				if ( dist < globals::aimbot_fov && dist < target_dist ) {
					target_dist = dist;
					target_entity = inc.actor;
				}
			}
		}*/

		if ( globals::static_box ) {

			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( head_box.x - ( box_width / 2 ), head_box.y ), ImVec2( root_screen.x + ( box_width / 2 ), head_box.y ), visible_color, 2 ); // T

			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( head_box.x - ( box_width / 2 ), head_box.y ), ImVec2( head_box.x - ( box_width / 2 ), root_screen.y ), visible_color, 2 ); // L

			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( head_box.x - ( box_width / 2 ), root_screen.y ), ImVec2( root_screen.x + ( box_width / 2 ), root_screen.y ), visible_color, 2 ); // T

			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( root_screen.x + ( box_width / 2 ), head_box.y ), ImVec2( root_screen.x + ( box_width / 2 ), root_screen.y ), visible_color, 2 ); // R
		}

		if ( globals::snapline ) ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( globals::screen_width / 2, globals::screen_height - 30 ), ImVec2( root_screen.x, root_screen.y + 20 ), visible_color, 2 );

		if ( globals::skeleton ) {

			auto neck = worldtoscreen( getbone( inc.skeletal_mesh, 66 ) );
			auto left_shoulder = worldtoscreen( getbone( inc.skeletal_mesh, 9 ) );
			auto left_elbow = worldtoscreen( getbone( inc.skeletal_mesh, 10 ) );
			auto left_hand = worldtoscreen( getbone( inc.skeletal_mesh, 11 ) );
			auto right_shoulder = worldtoscreen( getbone( inc.skeletal_mesh, 38 ) );
			auto right_elbow = worldtoscreen( getbone( inc.skeletal_mesh, 39 ) );
			auto right_hand = worldtoscreen( getbone( inc.skeletal_mesh, 40 ) );
			auto pelvis = worldtoscreen( getbone( inc.skeletal_mesh, 2 ) );
			auto left_hip = worldtoscreen( getbone( inc.skeletal_mesh, 71 ) );
			auto left_knee = worldtoscreen( getbone( inc.skeletal_mesh, 72 ) );
			auto left_foot = worldtoscreen( getbone( inc.skeletal_mesh, 73 ) );
			auto right_hip = worldtoscreen( getbone( inc.skeletal_mesh, 78 ) );
			auto right_knee = worldtoscreen( getbone( inc.skeletal_mesh, 79 ) );
			auto right_foot = worldtoscreen( getbone( inc.skeletal_mesh, 82 ) );

			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( head_screen.x, head_screen.y ), ImVec2( neck.x, neck.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( left_shoulder.x, left_shoulder.y ), ImVec2( right_shoulder.x, right_shoulder.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( left_shoulder.x, left_shoulder.y ), ImVec2( left_elbow.x, left_elbow.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( left_elbow.x, left_elbow.y ), ImVec2( left_hand.x, left_hand.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( right_shoulder.x, right_shoulder.y ), ImVec2( right_elbow.x, right_elbow.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( right_elbow.x, right_elbow.y ), ImVec2( right_hand.x, right_hand.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( neck.x, neck.y ), ImVec2( pelvis.x, pelvis.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( pelvis.x, pelvis.y ), ImVec2( left_hip.x, left_hip.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( left_hip.x, left_hip.y ), ImVec2( left_knee.x, left_knee.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( left_knee.x, left_knee.y ), ImVec2( left_foot.x, left_foot.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( pelvis.x, pelvis.y ), ImVec2( right_hip.x, right_hip.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( right_hip.x, right_hip.y ), ImVec2( right_knee.x, right_knee.y ), visible_color, 2 );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( right_knee.x, right_knee.y ), ImVec2( right_foot.x, right_foot.y ), visible_color, 2 );

		}
		float yOffsetTop = 10.0f;
		//if (globals::username)  // Ensure globals::username is properly defined and accessible
		//{
		//	ImVec2 text_size = ImGui::CalcTextSize(inc.username.c_str());
		//	ImVec2 pos = ImVec2(head_screen.x - text_size.x / 2, head_screen.y - yOffsetTop - text_size.y / 2);  // Position adjusted using head_screen
		//	ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
		//	ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

		//	ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 255), inc.username.c_str());
		//	yOffsetTop += 15.0f;
		//}
		if (globals::platform)
		{
			uintptr_t platform_ptr = read<uintptr_t>(inc.player_state + OFFSET_PLATFORM);
			wchar_t platform_char[64] = { 0 };

			kernel->read_phys(reinterpret_cast<PVOID>(platform_ptr), reinterpret_cast<uint8_t*>(platform_char), sizeof(platform_char));

			std::wstring platform_wide_str(platform_char);
			std::string platform_str(platform_wide_str.begin(), platform_wide_str.end());

			ImVec2 text_size = ImGui::CalcTextSize(platform_str.c_str());
			ImVec2 pos = ImVec2(head_screen.x - text_size.x / 2, head_screen.y - yOffsetTop - text_size.y / 2);  // Use head_screen for positioning
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			ImColor platform_col;
			if (platform_str == "XBL")
			{
				platform_col = ImColor(0, 255, 0, 255);
			}
			else if (platform_str == "PSN" || platform_str == "PS5")
			{
				platform_col = ImColor(0, 0, 255, 255);
			}
			else if (platform_str == "SWT")
			{
				platform_col = ImColor(255, 0, 0, 255);
			}
			else if (platform_str == "WIN")
			{
				platform_col = ImColor(255, 255, 255, 255);
			}

			ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 13.0f, pos, platform_col, platform_str.c_str());
		}

		float yOffsetBottom = 10.0f;
		if ( globals::distance )
		{
			std::string dist_str = ( std::string )skCrypt( "[" ) + std::to_string( ( int )distance ) + ( std::string )skCrypt( "m]" );
			auto text_size = ImGui::CalcTextSize( dist_str.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( ImVec2( root_screen.x - ( text_size.x / 2 ), root_screen.y + 5 ), visible_color, dist_str.c_str());

		}
		/*if (globals::rank)
		{
			uintptr_t habenaro = read<uintptr_t>(inc.player_state + offsets::habenaro_component);
			int32_t ranked_progress = read<int32_t>(habenaro + offsets::ranked_progress + 0x10);
			std::string rank_name = get_rank_name(ranked_progress);
			ImVec4 rank_color = get_rank_color(ranked_progress);

			ImVec2 text_size = ImGui::CalcTextSize(rank_name.c_str());
			ImVec2 pos = ImVec2(bottom2d.x - text_size.x / 2, bottom2d.y + yOffsetBottom - text_size.y / 2);
			ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
			ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

			ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 13.0f, pos, rank_color, rank_name.c_str());
			yOffsetBottom += 15.0f;
		}*/
	}

	/*if ( target_entity && globals::aimbot ) {

		auto closest_mesh = read<std::uint64_t>( target_entity + OFFSET_SKELETALMESH );

		fvector hitbox = getbone( closest_mesh, bone::head );
		fvector2d hitbox_screen;

		switch ( globals::hitbox ) {
		case 0:
			hitbox_screen = worldtoscreen( getbone( closest_mesh, bone::head ) );
			break;
		case 1:
			hitbox_screen = worldtoscreen( getbone( closest_mesh, bone::neck ) );
			break;
		case 2:
			hitbox_screen = worldtoscreen( getbone( closest_mesh, bone::chest ) );
			break;
		case 3:
			hitbox_screen = worldtoscreen( getbone( closest_mesh, bone::pelvis ) );
			break;
		}

		if ( hitbox.x != 0 || hitbox.y != 0 && ( get_cross_distance( hitbox.x, hitbox.y, globals::screen_width / 2, globals::screen_height / 2 ) <= globals::aimbot_fov ) )
		{
			if (GetAsyncKeyState(globals::aimbot_keyz))
				mouse->target_screen_pos(hitbox_screen.x, hitbox_screen.y);
		}

	}
	else {
		target_dist = FLT_MAX;
		target_entity = NULL;
	}*/

}