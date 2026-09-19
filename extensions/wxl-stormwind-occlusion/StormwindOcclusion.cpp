// wxl-stormwind-occlusion: disable the broken Stormwind occluder table entry.
// Copyright (C) 2026 WarcraftXL
// SPDX-License-Identifier: GPL-3.0-or-later

#include "wxl/PluginApi.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace
{
    constexpr uintptr_t kEntryAddress = 0xAF0038;
    constexpr uint32_t kDisabledMapId = 9999;

    struct OccluderEntry
    {
        uint32_t unk1;
        uint32_t unk2;
        uint32_t mapId;
        uint32_t unk3;
        uint32_t unk4;
        uint32_t unk5;
        uint32_t unk6;
        uint32_t unk7;
        uint32_t unk8;
        uint32_t unk9;
    };

    static_assert(sizeof(OccluderEntry) == 40);

    bool HasExpectedSignature(const OccluderEntry& entry)
    {
        return entry.unk1 == 0x450C3B33
            && entry.unk2 == 0x44198000
            && entry.unk3 == 0
            && entry.unk4 == 0
            && entry.unk5 == 0
            && entry.unk6 == 0
            && entry.unk7 == 0
            && entry.unk8 == 0
            && entry.unk9 == 0;
    }

    bool WriteMapId(uint32_t mapId)
    {
        void* const destination = reinterpret_cast<void*>(
            kEntryAddress + offsetof(OccluderEntry, mapId));

        DWORD previousProtection = 0;
        if (!VirtualProtect(destination, sizeof(mapId), PAGE_READWRITE, &previousProtection))
            return false;

        std::memcpy(destination, &mapId, sizeof(mapId));

        DWORD ignored = 0;
        return VirtualProtect(destination, sizeof(mapId), previousProtection, &ignored) != FALSE;
    }
}

extern "C" const WXL_PluginInfo* __cdecl WXL_Query()
{
    static const WXL_PluginInfo info = {
        sizeof(WXL_PluginInfo),
        WXL_API_VERSION,
        "stormwind-occlusion",
        1,
        WXL_CLIENT_BUILD,
    };
    return &info;
}

extern "C" int __cdecl WXL_Load(const WXL_Api* api)
{
    if (!api || !api->Log) return 0;

    OccluderEntry entry{};
    std::memcpy(&entry, reinterpret_cast<const void*>(kEntryAddress), sizeof(entry));

    if (!HasExpectedSignature(entry))
    {
        api->Log(WXL_LOG_ERROR, "stormwind-occlusion",
                 "the occluder table entry does not match client build 12340");
        return 0;
    }

    if (entry.mapId == kDisabledMapId)
    {
        api->Log(WXL_LOG_INFO, "stormwind-occlusion", "already applied");
        return 1;
    }

    if (entry.mapId != 0 || !WriteMapId(kDisabledMapId))
    {
        api->Log(WXL_LOG_ERROR, "stormwind-occlusion", "could not disable the occluder entry");
        return 0;
    }

    api->Log(WXL_LOG_INFO, "stormwind-occlusion", "Stormwind occluder entry disabled");
    return 1;
}
