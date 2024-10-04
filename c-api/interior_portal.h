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

EXPORT_CLIENT uint16_t InteriorPortal_GetCornerCount(uint32_t interiorId, uint32_t portalIndex);
EXPORT_CLIENT alt::Position InteriorPortal_GetCornerPosition(uint32_t interiorId, uint32_t portalIndex, uint32_t cornerIndex);
EXPORT_CLIENT uint32_t InteriorPortal_GetRoomFrom(uint32_t interiorId, uint32_t portalIndex);
EXPORT_CLIENT uint32_t InteriorPortal_GetRoomTo(uint32_t interiorId, uint32_t portalIndex);
EXPORT_CLIENT int32_t InteriorPortal_GetFlag(uint32_t interiorId, uint32_t portalIndex);

EXPORT_CLIENT uint16_t InteriorPortal_GetEntityCount(uint32_t interiorId, uint32_t portalIndex);
EXPORT_CLIENT uint32_t InteriorPortal_GetEntityArcheType(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex);
EXPORT_CLIENT int32_t InteriorPortal_GetEntityFlag(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex);
EXPORT_CLIENT alt::Position InteriorPortal_GetEntityPosition(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex);
EXPORT_CLIENT alt::Rotation InteriorPortal_GetEntityRotation(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex);


EXPORT_CLIENT void InteriorPortal_SetCornerPosition(uint32_t interiorId, uint32_t portalIndex, uint32_t cornerIndex, position_t position);
EXPORT_CLIENT void InteriorPortal_SetRoomFrom(uint32_t interiorId, uint32_t portalIndex, uint32_t roomFrom);
EXPORT_CLIENT void InteriorPortal_SetRoomTo(uint32_t interiorId, uint32_t portalIndex, uint32_t roomTo);
EXPORT_CLIENT void InteriorPortal_SetFlag(uint32_t interiorId, uint32_t portalIndex, int32_t flag);
EXPORT_CLIENT void InteriorPortal_SetEntityFlag(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex, int32_t flag);

#ifdef ALT_CLIENT_API
std::shared_ptr<alt::IInteriorPortal> GetInteriorPortal(uint32_t interiorId, uint32_t portalIndex);
#endif