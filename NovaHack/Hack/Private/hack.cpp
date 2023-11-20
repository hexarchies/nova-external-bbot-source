#include "../Public/hack.h"

engine::camera hack::GetCamera(engine::vec3 Rotation)
{
    engine::camera cam;

    cam.Location = memory::ReadMemory<engine::vec3>(cache::ViewMatrix + 0x240);

    cam.Rotation.x = (memory::ReadMemory<float>(cache::ViewMatrix + 0x228)) * 180 / engine::M_PI;
    cam.Rotation.y = Rotation.y;
    cam.Rotation.z = 0.0f;

    //if (cam.Rotation.x < 0)
    //    cam.Rotation.x = 360 + cam.Rotation.x;

    cam.FOV = memory::ReadMemory<float>(cache::PlayerCameraManager + (offsets::DefaultFOV - 0x4));
    //cam.FOV = 80.f;

    return cam;
}

bool hack::Attach()
{
    memory::GameProcessId = memory::GetProcessId(L"FortniteClient-Win64-Shipping.exe");
    if (!memory::GameProcessId) return false;

    memory::GameHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, memory::GameProcessId);
    if (!memory::GameHandle) return false;

    memory::GameBase = memory::GetModuleBaseAddress(memory::GameProcessId, L"FortniteClient-Win64-Shipping.exe");
    if (!memory::GameBase) return false;

    return true;
}

inline unsigned int dword_5A1DA78 = 0;
inline static void fname_decrypt(char* inputBuf, int namelength)
{
    char decname[1024];
    ZeroMemory(decname, sizeof(decname));

    __int64 outputBuff = reinterpret_cast<__int64>(&decname);

    if (dword_5A1DA78 == 0 && memory::GameBase != 0)
    {
        dword_5A1DA78 = memory::ReadMemory<unsigned int>(memory::GameBase + 0x5A1DA78, sizeof(dword_5A1DA78));
    }
    char v9;
    char v4 = *inputBuf;
    unsigned int v5 = dword_5A1DA78 % 0x21;
    unsigned int v6 = dword_5A1DA78 % 0x21 + (dword_5A1DA78 ^ 0x9C677CC5);
    *decname = static_cast<BYTE>(v6 ^ *inputBuf);
    if (v4 != (BYTE)v6)
    {
        char* v7 = decname;
        __int64 v8 = (uint64_t)inputBuf - outputBuff;
        do
        {
            v6 += v5 + (DWORD)++v7 - (DWORD)outputBuff;
            v9 = v6 ^ v7[v8];
            *v7 = v9;
        } while (v9);
    }


    memcpy(inputBuf, decname, sizeof(decname));
}
#define FName 0x5A1DA78

inline std::string GetNameFromFName(int key)
{


    constexpr auto ElementsPerChunk = 0x4000;

    enum { NAME_SIZE = 1024 };

    char name[NAME_SIZE] = { 0 };
    SecureZeroMemory(name, NAME_SIZE);


    const int32_t Index = key;


    const int32_t ChunkIndex = Index / ElementsPerChunk;
    const int32_t WithinChunkIndex = Index % ElementsPerChunk;

    const uint64_t ElementType = memory::ReadMemory<uint64_t>(FName + 8 * ChunkIndex + 112, sizeof(ElementType));

    const auto FNameEntryPtrPtr = ElementType + (WithinChunkIndex * 8);

    const auto FNameEntryPtr = memory::ReadMemory<uint64_t>(FNameEntryPtrPtr, sizeof(uint64_t));


    const uint64_t AnsiName = FNameEntryPtr + 0xC;


    int nameLength = NAME_SIZE - 1;
    ReadProcessMemory(memory::GameHandle, reinterpret_cast<void*>(AnsiName), name, nameLength, NULL);
    fname_decrypt(name, nameLength);
    std::string finalName = name;
    finalName = finalName.substr(0, nameLength);

    return finalName;
}

bool hack::Tick()
{
    cache::AimData.closestPawn = 0;

    cache::Uworld = memory::ReadMemory<uintptr_t>(memory::GameBase + offsets::Uworld);
    if (!cache::Uworld) return false;

    cache::PersistentLevel = memory::ReadMemory<uintptr_t>(cache::Uworld + offsets::PersistentLevel);
    if (!cache::PersistentLevel) return false;

    cache::GameState = memory::ReadMemory<uintptr_t>(cache::Uworld + offsets::GameState);
    if (!cache::GameState) return false;

    cache::GameInstance = memory::ReadMemory<uintptr_t>(cache::Uworld + offsets::GameInstance);
    if (!cache::GameInstance) return false;

    cache::LocalPlayers = memory::ReadMemory<uintptr_t>(cache::GameInstance + offsets::LocalPlayers);
    if (!cache::LocalPlayers) return false;

    cache::LocalPlayer = memory::ReadMemory<uintptr_t>(cache::LocalPlayers);
    if (!cache::LocalPlayer) return false;

    cache::PlayerController = memory::ReadMemory<uintptr_t>(cache::LocalPlayer + offsets::PlayerController);
    if (!cache::PlayerController) return false;

    cache::PlayerCameraManager = memory::ReadMemory<uintptr_t>(cache::PlayerController + offsets::PlayerCameraManager);
    if (!cache::PlayerCameraManager) return false;

    cache::Character = memory::ReadMemory<uintptr_t>(cache::PlayerController + offsets::Character);
    if (!cache::Character) return false;

    cache::Mesh = memory::ReadMemory<uintptr_t>(cache::Character + offsets::Mesh);
    if (!cache::Mesh) return false;

    cache::AcknowledgedPawn = memory::ReadMemory<uintptr_t>(cache::PlayerController + offsets::AcknowledgedPawn);
    if (!cache::AcknowledgedPawn) return false;

    cache::RootComponent = memory::ReadMemory<uintptr_t>(cache::AcknowledgedPawn + offsets::RootComponent);
    if (!cache::RootComponent) return false;

    cache::PlayerState = memory::ReadMemory<uintptr_t>(cache::AcknowledgedPawn + offsets::PlayerState);
    if (!cache::PlayerState) return false;

    cache::ViewMatrix = memory::ReadMemory<uintptr_t>(cache::LocalPlayer + offsets::ViewMatrix);
    if (!cache::ViewMatrix) return false;

    cache::PlayerArray = memory::ReadMemory<uintptr_t>(cache::GameState + offsets::PlayerArray);
    if (!cache::PlayerArray) return false;

    cache::PlayerCount = memory::ReadMemory<int>(cache::GameState + offsets::PlayerArray + sizeof(uintptr_t));
    if (!cache::PlayerCount) return false;

    cache::TeamIndex = memory::ReadMemory<engine::uint8>(cache::PlayerState + offsets::TeamIndex);
    cache::CurrentWeapon = memory::ReadMemory<uintptr_t>(cache::AcknowledgedPawn + offsets::CurrentWeapon);
    cache::WeaponMesh = memory::ReadMemory<uintptr_t>(cache::CurrentWeapon + offsets::WeaponMesh);
    cache::WeaponData = memory::ReadMemory<uintptr_t>(cache::CurrentWeapon + offsets::WeaponData);
    cache::CharacterMovement = memory::ReadMemory<uintptr_t>(cache::Character + offsets::CharacterMovement);

    cache::LocalLocation = memory::ReadMemory<engine::vec3>(cache::RootComponent + offsets::RelativeLocation);
    cache::LocalRotation = memory::ReadMemory<engine::vec3>(cache::RootComponent + offsets::RelativeRotation);

    cache::LocalCamera = GetCamera(cache::LocalRotation);

    cache::AimData.ClosestFovDistance = FLT_MAX;
    cache::AimData.ClosestAimPos3D = { 0.f, 0.f, 0.f };

    for (int i = 0; i < cache::PlayerCount; i++)
    {
        uintptr_t PlayerState = memory::ReadMemory<uintptr_t>(cache::PlayerArray + (i * sizeof(uintptr_t)));
        if (!PlayerState) continue;

        uintptr_t PawnPrivate = memory::ReadMemory<uintptr_t>(PlayerState + offsets::PawnPrivate);
        if (!PawnPrivate) continue;
        cache::CurrentActor = PawnPrivate;

        engine::uint8 TeamIndex = memory::ReadMemory<engine::uint8>(PlayerState + offsets::TeamIndex);
        if (PawnPrivate == cache::AcknowledgedPawn || TeamIndex == cache::TeamIndex) continue;

        uintptr_t Mesh = memory::ReadMemory<uintptr_t>(PawnPrivate + offsets::Mesh);
        if (!Mesh) continue;

        uintptr_t RootComponent = memory::ReadMemory<uintptr_t>(PawnPrivate + offsets::RootComponent);
        if (!RootComponent) continue;

        engine::vec3 RootPos3D = memory::ReadMemory<engine::vec3>(RootComponent + offsets::RelativeLocation);
        engine::vec2 RootPos2D{};

        if (!engine::WorldToScreen(RootPos3D, cache::LocalCamera, &RootPos2D)) continue;

        int WorldDistance = RootPos3D.DistanceInt(cache::LocalLocation);
        float FovDistance = RootPos2D.Distance(engine::ScreenCenter);

        if (FovDistance < cfg::DynamicSavedFOV)
        {
            if (FovDistance < cache::AimData.ClosestFovDistance)
            {
                cache::AimData.ClosestFovDistance = FovDistance;
                cache::AimData.ClosestAimPos3D = RootPos3D + engine::vec3(0.f, 0.f, 55.f);
                cache::AimData.closestPawn = PawnPrivate;
            }
        }
        engine::drawdata Data{ RootPos3D, WorldDistance, FovDistance, Mesh };
        cache::EspDataArray.push_back(Data);
    }

    //int actorcount = memory::ReadMemory<int>(cache::PersistentLevel + 0xA8);
    //for (int ii = 0; ii < actorcount; ii++)
    //{
    //    std::cout << "Inside Actor Loop" << std::endl;
    //    uintptr_t currentactor = memory::ReadMemory<uintptr_t>(0xA0 + ii * 0x8);
    //    int currentactorid = memory::ReadMemory<int>(currentactor + 0x18);

    //    engine::vec2 ScreenPos{};

    //    FVector Location = Engine::GetLocation(currentactor);
    //    if (!engine::WorldToScreen(Location, cache::LocalCamera, &ScreenPos)) continue;

    //    std::string ObjectName = GetNameFromFName(currentactorid);
    //    std::cout << ObjectName << std::endl;


    //}
    aim::Tick();
    esp::Tick();
    exploits::Tick();

    return true;
}
