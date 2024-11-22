#pragma once

#include "cpp-sdk/ICore.h"
#include "bone_info.h"

struct ClrVehicleModelInfo
{
    uint32_t modelHash;
    char* title = nullptr;
    alt::VehicleModelInfo::Type modelType;
    uint8_t wheelsCount;
    uint8_t hasArmoredWindows;
    uint8_t primaryColor;
    uint8_t secondaryColor;
    uint8_t pearlColor;
    uint8_t wheelsColor;
    uint8_t interiorColor;
    uint8_t dashboardColor;

    uint16_t extras;
    uint16_t defaultExtras;
    bool modkits[2];
    uint8_t hasAutoAttachTrailer;

    ClrBoneInfo* bones;
    uint32_t boneSize;

    uint8_t canAttachCars;
    uint32_t handlingNameHash;

    ClrVehicleModelInfo() = default;

    ClrVehicleModelInfo(alt::VehicleModelInfo info) :
        modelHash(info.modelHash),
        modelType(info.modelType),
        wheelsCount(info.wheelsCount),
        hasArmoredWindows(info.hasArmoredWindows),
        primaryColor(info.primaryColor),
        secondaryColor(info.secondaryColor),
        pearlColor(info.pearlColor),
        wheelsColor(info.wheelsColor),
        interiorColor(info.interiorColor),
        dashboardColor(info.dashboardColor),
        extras(info.extras),
        defaultExtras(info.defaultExtras),
        hasAutoAttachTrailer(info.hasAutoAttachTrailer),
        canAttachCars(info.canAttachCars),
        handlingNameHash(info.handlingNameHash)
    {
#ifdef _MSC_VER
        auto s_strcpy = [](char* dest, std::size_t n, const char* src) { return strcpy_s(dest, n, src); };
#else
        auto s_strcpy = [](char* dest, std::size_t n, const char* src) { return strncpy(dest, src, n); };
#endif
        size_t modkitsSize = std::size(info.modkits);
        for (size_t i = 0; i < modkitsSize; i++)
        {
            modkits[i] = info.modkits[i] != 0xFFFF;
        }

        title = new char[info.title.length() + 1];
        s_strcpy(title, info.title.length() + 1, info.title.c_str());

        const auto vehModelBones = info.bones;
        boneSize = vehModelBones.size();
        bones = new ClrBoneInfo[boneSize];
        for (uint32_t i = 0; i < boneSize; i++)
        {
            bones[i] = ClrBoneInfo(vehModelBones[i]);
        }
    }

    ~ClrVehicleModelInfo()
    {
        delete[] title;
    }
};
