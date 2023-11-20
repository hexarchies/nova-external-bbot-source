#pragma once
#include "memory.h"
#include "engine.h"
#include "cache.h"
#include "offsets.h"
#include "exploits.h"
#include "aim.h"
#include "esp.h"

#define FVector engine::vec3

inline namespace Engine
{
	inline FortPTR RootComponent(FortPTR actor)
	{
		return memory::ReadMemory<FortPTR>(actor + offsets::RootComponent);
	}
	inline FVector GetLocation(FortPTR actor)
	{
		return memory::ReadMemory<FVector>(RootComponent(actor) + offsets::RelativeLocation);
	}
}

class hack
{
    static engine::camera GetCamera(engine::vec3 Rotation);
public:

    static bool Attach();
    static bool Tick();
};

