#include "../Public/menu.h"
#include "../Public/render.h"

#include "../../globals.h"
#include "../../custom.h"
#include "../../bytearray.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "d3dx9.lib")

#include <Windows.h>

static int m_tabs;
static bool toggled = true;
static float open_alpha = 0;

// Fonts
ImFont* tab_title;
ImFont* font_icon;
ImFont* poppins;

// Byte
IDirect3DTexture9* bgs = nullptr;
IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* logotwo = nullptr;
IDirect3DTexture9* foto_user = nullptr;

namespace render_ui
{
    void rage_tab()
    {
     

        ImGui::SetCursorPos(ImVec2(169, 134));
        ImGui::MenuChild("General", ImVec2(320, 240));
        {
            static bool active = true;
            static bool inactive = false;

            ImGui::Spacing();
            ImGui::Checkbox("Enable Particles", &particles);

            static int procent = 60;
            ImGui::SliderInt("Fov", &procent, 0, 100, "%d%%");

            ImGui::Checkbox("Autofire", &inactive);
            ImGui::Checkbox("Autowall", &active);
            ImGui::Checkbox("Silent aim", &active);
            ImGui::Checkbox("Quick peek", &active);
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(169, 413));
        ImGui::MenuChild("Exploits", ImVec2(320, 107));
        {
            static bool active = true;
            static bool inactive = false;

            ImGui::Spacing();
            ImGui::Checkbox("Lag Peek", &active);
            ImGui::Checkbox("Hide Shots", &active);
            ImGui::Checkbox("Double Tab", &active);
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(505, 38));
        ImGui::MenuChild("Accuracy", ImVec2(320, 286));
        {
            static bool active = true;
            static bool inactive = false;

            const char* type[] = { "Type", "Types", "Select" };
            static int combo;

            ImGui::Spacing();
            ImGui::Checkbox("Automatic stop", &active);

            ImGui::Combo("Combo", &combo, type, IM_ARRAYSIZE(type));

            ImGui::Checkbox("Hitchance", &active);

            static int procentw = 60;
            ImGui::SliderInt("Hitchance value", &procentw, 0, 100, "%d%%");

            static int procente = 20;
            ImGui::SliderInt("Damage value", &procente, 0, 100, "%d%%");

            static int procent = 56;
            ImGui::SliderInt("Damage override value", &procent, 0, 100, "%d%%");
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(505, 363));
        ImGui::MenuChild("Misc", ImVec2(320, 157));
        {
            static bool active = true;
            static bool inactive = false;

            ImGui::Spacing();
            ImGui::Checkbox("Prefer a pont", &inactive);
            ImGui::Checkbox("Prefer body aim", &active);
            ImGui::Checkbox("ignore limbs when moving", &active);
        }
        ImGui::EndChild();

    }

    void Particles()
    {
        if (!particles)
            return;

        ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

        static ImVec2 particle_pos[100];
        static ImVec2 particle_target_pos[100];
        static float particle_speed[100];
        static float particle_radius[100];


        for (int i = 1; i < 50; i++)
        {
            if (particle_pos[i].x == 0 || particle_pos[i].y == 0)
            {
                particle_pos[i].x = rand() % (int)screen_size.x + 1;
                particle_pos[i].y = 15.f;
                particle_speed[i] = 1 + rand() % 25;
                particle_radius[i] = rand() % 4;

                particle_target_pos[i].x = rand() % (int)screen_size.x;
                particle_target_pos[i].y = screen_size.y * 2;
            }

            particle_pos[i] = ImLerp(particle_pos[i], particle_target_pos[i], ImGui::GetIO().DeltaTime * (particle_speed[i] / 60));

            if (particle_pos[i].y > screen_size.y)
            {
                particle_pos[i].x = 0;
                particle_pos[i].y = 0;
            }

            ImGui::GetWindowDrawList()->AddCircleFilled(particle_pos[i], particle_radius[i], ImColor(174, 139, 148, 255 / 2));
        }

    }

    void RenderTab()
    {
        auto draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();

        draw->AddText(poppins, 17, ImVec2(pos.x + 13, pos.y + 81), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Aimbot");  // right bg
        draw->AddText(poppins, 17, ImVec2(pos.x + 13, pos.y + 210), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Visuals");  // right bg
        draw->AddText(poppins, 17, ImVec2(pos.x + 13, pos.y + 348), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), "Miscellaneous");  // right bg

        ImGui::SetCursorPos(ImVec2(13, 99));
        if (ImGui::Rendertab("r", "Legit Bot", !m_tabs)) m_tabs = 0;

        ImGui::SetCursorPos(ImVec2(13, 136));
        if (ImGui::Rendertab("e", "Rage Bot", m_tabs == 1)) m_tabs = 1;

        ImGui::SetCursorPos(ImVec2(13, 174));
        if (ImGui::Rendertab("a", "Anti-Aims", m_tabs == 2)) m_tabs = 2;

        ImGui::SetCursorPos(ImVec2(13, 228));
        if (ImGui::Rendertab("x", "Players", m_tabs == 3)) m_tabs = 3;

        ImGui::SetCursorPos(ImVec2(13, 266));
        if (ImGui::Rendertab("w", "World", m_tabs == 4)) m_tabs = 4;

        ImGui::SetCursorPos(ImVec2(13, 304));
        if (ImGui::Rendertab("v", "View", m_tabs == 5)) m_tabs = 5;

        ImGui::SetCursorPos(ImVec2(13, 369));
        if (ImGui::Rendertab("z", "Main", m_tabs == 6)) m_tabs = 6;

        ImGui::SetCursorPos(ImVec2(13, 407));
        if (ImGui::Rendertab("s", "Inventory", m_tabs == 7)) m_tabs = 7;

        ImGui::SetCursorPos(ImVec2(13, 445));
        if (ImGui::Rendertab("c", "Configs", m_tabs == 8)) m_tabs = 8;

        switch (m_tabs)
        {
        case 0: rage_tab(); break; // RAGEBOT
        case 1: break; // LEGITBOT
        case 2: break; // PLAYERS
        case 3: break; // VISUALS
        case 4: break; // MISC
        case 5: break; // SETTINGS
        }
    }

    void user_info()
    {
        auto draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();

        draw->AddRectFilled(ImVec2(pos.x + 9, pos.y + 486), ImVec2(pos.x + 152, pos.y + 523), ImColor(41, 41, 41, int(255 * ImGui::GetStyle().Alpha)), 5.f, ImDrawCornerFlags_All); // right bg
        draw->AddRect(ImVec2(pos.x + 9, pos.y + 486), ImVec2(pos.x + 152, pos.y + 523), ImColor(50, 50, 50, int(255 * ImGui::GetStyle().Alpha)), 5.f, ImDrawCornerFlags_All, 1);  // right bg

        //парсим с сайта
        draw->AddColoredText(ImVec2(pos.x + 49, pos.y + 488), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), ImColor(255, 255, 255, int(255 * ImGui::GetStyle().Alpha)), "(bouy)");
        draw->AddColoredText(ImVec2(pos.x + 49, pos.y + 503), ImColor(105, 105, 105, int(255 * ImGui::GetStyle().Alpha)), ImColor(255, 255, 255, int(255 * ImGui::GetStyle().Alpha)), "Till: (25.09.2023)");

        ImGui::SetCursorPos(ImVec2(15, 490));
        ImGui::Image(foto_user, ImVec2(28, 28));//logo 2
    }

    void Decoration()
    {
        auto draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();

        draw->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 161, pos.y + 535), ImColor(32, 32, 32, int(255 * ImGui::GetStyle().Alpha)), 10.f, ImDrawCornerFlags_Left); // left bg
        draw->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + 161, pos.y + 535), ImColor(50, 50, 50, int(255 * ImGui::GetStyle().Alpha)), 10.f, ImDrawCornerFlags_Left, 1);  // left bg

        draw->AddRectFilled(ImVec2(pos.x + 160, pos.y), ImVec2(pos.x + 838, pos.y + 535), ImColor(26, 26, 26, int(255 * ImGui::GetStyle().Alpha)), 10.f, ImDrawCornerFlags_Right); // right bg
        draw->AddRect(ImVec2(pos.x + 160, pos.y), ImVec2(pos.x + 838, pos.y + 535), ImColor(50, 50, 50, int(255 * ImGui::GetStyle().Alpha)), 10.f, ImDrawCornerFlags_Right, 1);  // right bg

        // на будующие делаю 2 лого не ебу какое вам нужно
        ImGui::SetCursorPos(ImVec2(23, 30));
        ImGui::Image(logo, ImVec2(120, 25.1f));//logo 1

        ImGui::SetCursorPos(ImVec2(36, -7));
        ImGui::Image(logotwo, ImVec2(85, 98));//logo 2
    }

    void Render()
    {

        if (GetAsyncKeyState(VK_INSERT) & 1)
            toggled = !toggled;

        open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.5f : -1.5f)), 0.f, 1.f);
        if (open_alpha > 0.01f)
        {
            ImGui::SetNextWindowSize(ImVec2(838 * dpi_scale, 535 * dpi_scale));
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, open_alpha);
            ImGui::Begin("Dear ImGui", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {
                Decoration();
                RenderTab();
                user_info();

                Particles();
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
    }

};

bool menu::InitMenu(IDirect3DDevice9* device)
{
    auto& style = ImGui::GetStyle();
    auto& color = style.Colors;
    ImGuiIO& io = ImGui::GetIO();

    style.FramePadding = ImVec2(1, 0);
    style.FrameRounding = 3;
    style.WindowBorderSize = 0;
    style.ScrollbarRounding = 3;
    style.ScrollbarSize = 5;

    // Setup Platform/Renderer backends
    //ImGui_ImplWin32_Init(hwnd);
    //ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    //Font
    font_config.GlyphRanges = ranges;

    io.Fonts->AddFontFromMemoryTTF(poppin_font, sizeof(poppin_font), 16, &font_config, ranges);
    tab_title = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 19.0f, &font_config, ranges);
    font_icon = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 25.0f, &font_config, ranges);
    poppins = io.Fonts->AddFontFromMemoryTTF(poppin_font, sizeof(poppin_font), 25.0f, &font_config, ranges);

    //Byte
    if (bgs == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(device, bg_one, sizeof(bg_one), 1920, 1080, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bgs);

    if (logo == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(device, logo_one, sizeof(logo_one), 120, 25.1, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &logo);

    if (logotwo == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(device, logo_two, sizeof(logo_two), 85, 98, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &logotwo);

    if (foto_user == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(device, user, sizeof(user), 28, 28, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &foto_user);

	return false;
}

bool menu::RenderMenu()
{
	render_ui::Render();  //Render

	return true;
}
