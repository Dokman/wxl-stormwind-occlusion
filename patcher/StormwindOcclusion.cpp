// wxl-stormwind-occlusion: disable the broken Stormwind occluder table entry.
// Copyright (C) 2026 WarcraftXL
// SPDX-License-Identifier: GPL-3.0-or-later

#include "patcher/PatchScript.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace
{
    using namespace wxl::patcher;

    constexpr uint32_t kEntryVa = 0xAF0038;
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

    bool ReadEntry(PeImage& pe, OccluderEntry& entry)
    {
        const uint32_t offset = pe.VaToOffset(kEntryVa);
        const auto& bytes = pe.bytes();
        if (offset == 0 || static_cast<size_t>(offset) + sizeof(entry) > bytes.size()) return false;
        std::memcpy(&entry, bytes.data() + offset, sizeof(entry));
        return true;
    }

    bool HasExpectedSignature(const OccluderEntry& entry)
    {
        // Validate every invariant field from the stock 12340 entry. mapId is handled
        // separately so applying the patch a second time remains a successful no-op.
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

    class StormwindOcclusion final : public PatchScript
    {
    public:
        const char* name() const override { return "stormwind-occlusion"; }

        bool Apply(PeImage& pe) const override
        {
            OccluderEntry entry{};
            if (!ReadEntry(pe, entry) || !HasExpectedSignature(entry)) return false;
            if (entry.mapId == kDisabledMapId) return true;
            if (entry.mapId != 0) return false;

            return pe.WriteVa(kEntryVa + offsetof(OccluderEntry, mapId),
                              &kDisabledMapId, sizeof(kDisabledMapId));
        }
    };

    const StormwindOcclusion g_stormwindOcclusion;
}
