#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wempty-body"
#pragma clang diagnostic ignored "-Wswitch"
#endif

#include "cpp-sdk/SDK.h"
#include "data/interior_extend_info.h"
#include "data/types.h"
#include "data/invoker.h"
#include "utils/export.h"

#ifdef ALT_SERVER_API
#include <CSharpResourceImpl.h>
#elif ALT_CLIENT_API
#include "../client/src/runtime/CSharpResourceImpl.h"
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

EXPORT_CLIENT uint32_t InteriorRoom_GetIndex(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex);
EXPORT_CLIENT const char* InteriorRoom_GetName(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, int32_t& size);
EXPORT_CLIENT uint32_t InteriorRoom_GetNameHash(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex);
EXPORT_CLIENT int32_t InteriorRoom_GetFlag(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex);
EXPORT_CLIENT uint32_t InteriorRoom_GetTimecycle(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex);

EXPORT_CLIENT void InteriorRoom_GetExtents(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, interior_extend_info_t& extents);

EXPORT_CLIENT void InteriorRoom_SetFlag(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, int32_t flag);
EXPORT_CLIENT void InteriorRoom_SetTimecycle(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, uint32_t timecycle);
EXPORT_CLIENT void InteriorRoom_SetExtents(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, position_t min, position_t max);


#ifdef ALT_CLIENT_API
std::shared_ptr<alt::IInteriorRoom> GetInteriorRoom(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex);
#endif