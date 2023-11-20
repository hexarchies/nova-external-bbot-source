#include "../Public/esp.h"

bool esp::IsVisible(uintptr_t Mesh)
{
    float LastSubmitTime = memory::ReadMemory<float>(Mesh + 0x28C);
    float LastRenderTimeOnScreen = memory::ReadMemory<float>(Mesh + 0x294);

    return LastRenderTimeOnScreen + 10.f >= LastSubmitTime;
}

bool esp::Tick()
{
    ImDrawList* Draw = ImGui::GetForegroundDrawList();
    if (!Draw) return false;

    for (int i = 0; i < cache::EspDataArray.size(); i++)
    {
        ImColor Color;
        engine::drawdata Data = cache::EspDataArray[i];

        float DefaultFOV = memory::ReadMemory<float>(cache::PlayerCameraManager + (offsets::DefaultFOV - 0x4));

        cfg::DynamicSavedFOV = (DefaultFOV - 80) + cfg::AimBotFOV;

        ImGui::GetForegroundDrawList()->AddCircle(
            ImVec2(engine::Screen.x / 2.f, engine::Screen.y / 2.f),
            cache::LocalCamera.FOV + cfg::AimBotFOV,
            ImColor(0, 0, 0, 255),
            0,
            2.f
        );

        cache::PlayersMesh = Data.Mesh;

        if (IsVisible(Data.Mesh)) // visible check
        {
            if (Data.FovDistance < cfg::DynamicSavedFOV)
                Color = ImColor(255, 0, 0, 255);
            else
                Color = ImColor(255, 165, 0, 255);
        }
        else
        {
            Color = ImColor(0, 255, 0, 255);
        }

        engine::vec3 HeadPos3D = Data.RootPos3D + engine::vec3(0.f, 0.f, 90.f);
        engine::vec3 FootPos3D = Data.RootPos3D - engine::vec3(0.f, 0.f, 90.f);
        engine::vec3 AimPos3D = Data.RootPos3D + engine::vec3(0.f, 0.f, 60.f);

        engine::vec2 HeadPos2D{};
        engine::vec2 FootPos2D{};

        if (!engine::WorldToScreen(HeadPos3D, cache::LocalCamera, &HeadPos2D)) continue;
        if (!engine::WorldToScreen(FootPos3D, cache::LocalCamera, &FootPos2D)) continue;
        float posY = FootPos2D.y;

        if (cfg::Snaplines) { // snaplines
            Draw->AddLine
            (
                ImVec2(FootPos2D.x, FootPos2D.y),
                ImVec2(engine::ScreenCenter.x, engine::ScreenCenter.y),
                ImColor(0, 0, 0, 255),
                1.f
            );
        }

        if (cfg::EspBox) { // espboxes
            float Width = (HeadPos2D.y - FootPos2D.y) * 0.50f;

            Draw->AddRect
            (
                ImVec2(HeadPos2D.x - Width, HeadPos2D.y),
                ImVec2(FootPos2D.x + Width, FootPos2D.y),
                ImColor(0, 0, 0, 255),
                0.f,
                0,
                1.f
            );
        }

        if (cfg::Reloading) {
            FortPTR PlayerWeapon = memory::ReadMemory<FortPTR>(cache::CurrentActor + offsets::CurrentWeapon);
            if (PlayerWeapon)
            {
                bool IsReloading = memory::ReadMemory<bool>(PlayerWeapon + offsets::bIsReloadingWeapon);
                if (IsReloading)
                {
                    Draw->AddText({ FootPos2D.x, posY }, Color, "Reloading");
                    posY += 15;
                }
            }
        }

        if (cfg::Targeting) {
            FortPTR PlayerWeapon = memory::ReadMemory<FortPTR>(cache::CurrentActor + offsets::CurrentWeapon);
            if (PlayerWeapon)
            {
                bool IsTargeting = memory::ReadMemory<bool>(PlayerWeapon + offsets::bIsTargeting);
                if (IsTargeting)
                {
                    Draw->AddText({ FootPos2D.x, posY }, ImColor(0, 0, 0, 255),  "Targeting");
                    posY += 15;
                }
            }           
        }

        if (cfg::ShowAmmo) {
            FortPTR PlayerWeapon = memory::ReadMemory<FortPTR>(cache::CurrentActor + offsets::CurrentWeapon);
            if (PlayerWeapon)
            {
                int AmmoCount = memory::ReadMemory<int32_t>(PlayerWeapon + offsets::AmmoCount);
                Draw->AddText({ FootPos2D.x, posY }, Color, std::string("Ammo: " + std::to_string(AmmoCount)).c_str());
                posY += 15;
            }
        }

        if (cfg::ShowDistance) {
            Draw->AddText({ FootPos2D.x, posY }, Color, std::string(std::to_string(Data.WorldDistance / 100) + 'm').c_str());
            posY += 15;
        }
    }

    cache::EspDataArray.clear();
    return true;
}
