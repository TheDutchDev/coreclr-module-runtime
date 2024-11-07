#include "interior_portal.h"

#include "cpp-sdk/script-objects/IInteriorPortal.h"
#include "utils/macros.h"

CAPI_START()

#ifdef ALT_CLIENT_API
uint32_t InteriorPortal_GetIndex(uint32_t interiorId, uint32_t portalIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetIndex();
}

uint16_t InteriorPortal_GetCornerCount(uint32_t interiorId, uint32_t portalIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetCornerCount();
}

alt::Position InteriorPortal_GetCornerPosition(uint32_t interiorId, uint32_t portalIndex, uint32_t cornerIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetCornerPosition(cornerIndex);
}

uint32_t InteriorPortal_GetRoomFrom(uint32_t interiorId, uint32_t portalIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetRoomFrom();
}

uint32_t InteriorPortal_GetRoomTo(uint32_t interiorId, uint32_t portalIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetRoomTo();
}

int32_t InteriorPortal_GetFlag(uint32_t interiorId, uint32_t portalIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetFlag();
}

uint16_t InteriorPortal_GetEntityCount(uint32_t interiorId, uint32_t portalIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetEntityCount();
}

uint32_t InteriorPortal_GetEntityArcheType(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetEntityArcheType(entityIndex);
}

int32_t InteriorPortal_GetEntityFlag(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetEntityFlag(entityIndex);
}

alt::Position InteriorPortal_GetEntityPosition(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetEntityPosition(entityIndex);
}

alt::Rotation InteriorPortal_GetEntityRotation(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	return portal->GetEntityRotation(entityIndex);
}

void InteriorPortal_SetCornerPosition(uint32_t interiorId, uint32_t portalIndex, uint32_t cornerIndex,
                                      position_t position)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	portal->SetCornerPosition(cornerIndex, alt::Position(position.x, position.y, position.z));
}

void InteriorPortal_SetRoomFrom(uint32_t interiorId, uint32_t portalIndex, uint32_t roomFrom)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	portal->SetRoomFrom(roomFrom);
}

void InteriorPortal_SetRoomTo(uint32_t interiorId, uint32_t portalIndex, uint32_t roomTo)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	portal->SetRoomTo(roomTo);
}

void InteriorPortal_SetFlag(uint32_t interiorId, uint32_t portalIndex, int32_t flag)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	portal->SetFlag(flag);
}

void InteriorPortal_SetEntityFlag(uint32_t interiorId, uint32_t portalIndex, uint32_t entityIndex, int32_t flag)
{
	auto portal = GetInteriorPortal(interiorId, portalIndex);
	portal->SetEntityFlag(entityIndex, flag);
}

std::shared_ptr<alt::IInteriorPortal> GetInteriorPortal(uint32_t interiorId, uint32_t portalIndex)
{
	auto interior = alt::ICore::Instance().GetInterior(interiorId);
	return interior->GetPortalByIndex(portalIndex);
}
#endif