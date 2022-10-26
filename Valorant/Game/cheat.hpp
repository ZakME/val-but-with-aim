#pragma once
#include "sdk.hpp"
#include <iostream>

using namespace Globals;
using namespace Camera;
using namespace UE4;
bool aimbot;
float aimbotfov = 10.0f;
bool circle;

GWorld* UWorld;
GGameInstance* UGameInstance;
GLocalPlayer* ULocalPlayer;
GPlayerController* APlayerController;
GPawn* APawn;
GPrivatePawn* APrivatePawn;
GULevel* ULevel;
GUSkeletalMeshComponent* USkeletalMeshComponent;

bool cached = false;
uintptr_t WorldPtr ;

class fvector {
public:
	fvector() : x(0.f), y(0.f), z(0.f)
	{

	}

	fvector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~fvector()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(fvector v) {
		return x * v.x + y * v.y + z * v.z;
	}

	inline fvector& operator-=(const fvector& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	inline fvector& operator+=(const fvector& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	inline fvector operator/(float v) const {
		return fvector(x / v, y / v, z / v);
	}

	inline float distance(fvector v) {
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline float lenght() {
		return sqrt(x * x + y * y + z * z);
	}

	bool operator==(fvector v) {
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}

	fvector operator+(fvector v) {
		return fvector(x + v.x, y + v.y, z + v.z);
	}

	fvector operator-(fvector v) {
		return fvector(x - v.x, y - v.y, z - v.z);
	}

	fvector operator*(float v) {
		return fvector(x * v, y * v, z * v);
	}

	fvector operator/(float v) {
		return fvector(x / v, y / v, z / v);
	}
};

//ue4 fquat class
struct fquat {
	float x;
	float y;
	float z;
	float w;
};

//ue4 ftransform class
struct ftransform {
	fquat rot;
	fvector translation;
	char pad[4];
	fvector scale;
	char pad1[4];
	D3DMATRIX ToMatrixWithScale() {
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

struct player
{
	uintptr_t for_actor;
	uintptr_t for_mesh;

	uintptr_t actor;
	uintptr_t mesh;
	uintptr_t bone_array;
	uintptr_t root_component;
	uintptr_t damage_handler;

	INT32 bone_count;
	INT32 ammo_count;

	std::string weapon_name;
	std::string agent_name;
	std::string player_name;

	float distance;
	float health;
	float shield;

	bool is_valid;
	bool is_damage_handler_guarded;
	bool is_mesh_guarded;
};

boolean in_rect(double centerX, double centerY, double radius, double x, double y) {
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}

int center_x = GetSystemMetrics(0) / 2 - 3;
int center_y = GetSystemMetrics(1) / 2 - 3;


auto CacheGame() -> void
{
	auto guardedregion = driver.guarded_region();

	while (true)
	{
		std::vector<ValEntity> CachedList;

		WorldPtr = GetWorld(guardedregion);

		auto ULevelPtr = UWorld->ULevel(WorldPtr);
		auto UGameInstancePtr = UWorld->GameInstance(WorldPtr);

		auto ULocalPlayerPtr = UGameInstance->ULocalPlayer(UGameInstancePtr);
		auto APlayerControllerPtr = ULocalPlayer->APlayerController(ULocalPlayerPtr);

		PlayerCameraManager = APlayerController->APlayerCameraManager(APlayerControllerPtr);
		auto MyHUD = APlayerController->AHUD(APlayerControllerPtr);

		auto APawnPtr = APlayerController->APawn(APlayerControllerPtr);

		if (APawnPtr != 0)
		{
			MyUniqueID = APawn->UniqueID(APawnPtr);
			MyRelativeLocation = APawn->RelativeLocation(APawnPtr);
		}

		if (MyHUD != 0)
		{
			auto PlayerArray = ULevel->AActorArray(ULevelPtr);

			for (uint32_t i = 0; i < PlayerArray.Count; ++i)
			{
				auto Pawns = PlayerArray[i];
				if (Pawns != APawnPtr)
				{
					if (MyUniqueID == APawn->UniqueID(Pawns))
					{
						ValEntity Entities{ Pawns };
						CachedList.push_back(Entities);
					}
				}
			}

			ValList.clear();
			ValList = CachedList;
			Sleep(1000);
		}
	}
}

std::vector<player> player_pawns;

namespace guarded_pointers
{
	uintptr_t guard_apawn = 0x0;
	uintptr_t guard_controller = 0x0;
}

namespace camera
{
	fvector location;
	fvector rotation;
	float fov;
}

namespace pointer
{
	uintptr_t apawn;
	uintptr_t apawn_old;
	uintptr_t player_controller;
	uintptr_t camera_manager;
}

auto CheatLoop() -> void
{
	for (ValEntity ValEntityList : ValList)
	{
		auto SkeletalMesh = APrivatePawn->USkeletalMeshComponent(ValEntityList.Actor);

		auto RelativeLocation = APawn->RelativeLocation(ValEntityList.Actor);
		auto RelativeLocationProjected = UE4::SDK::ProjectWorldToScreen(RelativeLocation);

		auto RelativePosition = RelativeLocation - CameraLocation;
		auto RelativeDistance = RelativePosition.Length() / 10000 * 2;

		auto HeadBone = UE4::SDK::GetEntityBone(SkeletalMesh, 8);
		auto HeadBoneProjected = UE4::SDK::ProjectWorldToScreen(HeadBone);

		auto RootBone = UE4::SDK::GetEntityBone(SkeletalMesh, 0);
		auto RootBoneProjected = UE4::SDK::ProjectWorldToScreen(RootBone);
		auto RootBoneProjected2 = UE4::SDK::ProjectWorldToScreen(FVector(RootBone.x, RootBone.y, RootBone.z - 15));

		auto Distance = MyRelativeLocation.Distance(RelativeLocation);

		float BoxHeight = abs(HeadBoneProjected.y - RootBoneProjected.y);
		float BoxWidth = BoxHeight * 0.40;

		auto ESPColor = ImColor(255, 0, 255);

		auto Health = APawn->Health(ValEntityList.Actor);
		if (Health <= 0) continue;
	
		if (APawn->bIsDormant(ValEntityList.Actor))
		{
			if (Settings::Visuals::bSnaplines)
				DrawTracers(RootBoneProjected, ESPColor);

			if (Settings::Visuals::bBox)
				Draw2DBox(RelativeLocationProjected, BoxWidth, BoxHeight, ESPColor);

			if (Settings::Visuals::bBoxOutlined)
				DrawOutlinedBox(RelativeLocationProjected, BoxWidth, BoxHeight, ESPColor);

			if (Settings::Visuals::bHealth)
				DrawHealthBar(RelativeLocationProjected, BoxWidth, BoxHeight, Health, RelativeDistance);

			if (Settings::Visuals::bDistance)
				DrawDistance(RootBoneProjected2, Distance);

			if (Settings::Visuals::bboxv2)
				renderBox(RelativeLocationProjected, RelativeDistance);
			if (Settings::Visuals::bDistance)
				DrawDistance(RelativeLocationProjected, RelativeDistance);



		}

		// if (GetAsyncKeyState(VK_RBUTTON) && in_rect(center_x, center_y, settings::aimbot_fov, head_screen.x, head_screen.y))
		if (aimbot)
		{

			auto HeadBone = UE4::SDK::GetEntityBone(SkeletalMesh, 8);
			auto HeadBoneProjected = UE4::SDK::ProjectWorldToScreen(HeadBone);

			if (GetAsyncKeyState(VK_RBUTTON) && in_rect(center_x, center_y, aimbotfov, HeadBone.x, HeadBone.y))
			{
				fvector control_rotation = driver.readv<fvector>(guarded_pointers::guard_controller + pointer::player_controller + 0x440);
				fvector camera_rotation = camera::rotation;
				fvector new_aim_rotation;
				fvector recoil;

				recoil.x = camera_rotation.x - control_rotation.x;
				recoil.y = camera_rotation.y - control_rotation.y;
				recoil.z = 0.f;


			}
		}
	}
}
