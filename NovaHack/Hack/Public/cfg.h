#pragma once

class cfg
{
public:
	// visuals
	inline static float DynamicSavedFOV;
	inline static bool EspBox = true;
	inline static bool Snaplines = false;
	inline static bool ShowDistance = true;
	inline static bool ShowWeapon = false;
	inline static bool ShowAmmo = false;
	inline static bool Reloading = false;
	inline static bool Targeting = false;

	inline static bool Crosshair = true;
	inline static float CrosshairLength = 20.f;

	// aim
	inline static bool Aimbot = true;
	inline static float AimSmoothness = 3.f;
	inline static float AimBotFOV = 200.f;

	// exploits
    inline static bool Exploits = false;
	inline static bool RapidFire = false;
	inline static bool NoRecoil = false;
	inline static bool NoSpread = false;
	inline static bool ADSAir = false;
	inline static bool Spinbot = false;
	inline static bool InstaRevive = false;
	inline static bool FullAutoWeapons = false;
	inline static bool WeaponSpinbot = false;
	inline static bool DoublePump = false;
	inline static bool PlayerFly = false;
	inline static bool AirStuck = false;
	inline static bool NoMinigunCooldown = false;
	inline static bool FirstPerson = false;
	inline static bool Backtrack = false;
	inline static bool FovChanger = false;

	inline static bool ChangeWeaponMesh = false;
	inline static bool ChangePlayerMesh = false;
	inline static bool ChangePlayersMesh = false;


	inline static float FovChangerScale = 90.f;
	inline static float WeaponMeshScale = 1.f;
	inline static float PlayerMeshScale = 1.f;
	inline static float PlayersMeshScale = 1.f;
};

