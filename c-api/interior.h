#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wempty-body"
#pragma clang diagnostic ignored "-Wswitch"
#endif

#include "cpp-sdk/SDK.h"
#include "data/interior_extent_info.h"
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

EXPORT_CLIENT uint16_t Interior_GetRoomCount(uint32_t interiorId);
EXPORT_CLIENT uint16_t Interior_GetPortalCount(uint32_t interiorId);
EXPORT_CLIENT alt::Position Interior_GetPosition(uint32_t interiorId);
EXPORT_CLIENT alt::Rotation Interior_GetRotation(uint32_t interiorId);
EXPORT_CLIENT void Interior_GetEntitiesExtents(uint32_t interiorId, interior_extent_info_t& extents);