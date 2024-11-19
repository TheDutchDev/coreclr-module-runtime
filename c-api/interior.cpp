#include "interior.h"

#include "utils/macros.h"

CAPI_START()

#ifdef ALT_CLIENT_API
uint16_t Interior_GetRoomCount(uint32_t interiorId)
{
	auto interior = alt::ICore::Instance().GetInterior(interiorId);
	return interior->GetRoomCount();
}

uint16_t Interior_GetPortalCount(uint32_t interiorId)
{
	auto interior = alt::ICore::Instance().GetInterior(interiorId);
	return interior->GetPortalCount();
}

alt::Position Interior_GetPosition(uint32_t interiorId)
{
	auto interior = alt::ICore::Instance().GetInterior(interiorId);
	return interior->GetPosition();
}

alt::Rotation Interior_GetRotation(uint32_t interiorId)
{
	auto interior = alt::ICore::Instance().GetInterior(interiorId);
	return interior->GetRotation();
}

void Interior_GetEntitiesExtents(uint32_t interiorId, aabb_t& extents)
{
	auto interior = alt::ICore::Instance().GetInterior(interiorId);
	auto ext = interior->GetEntitiesExtents();
	extents.min = ext.min;
	extents.max = ext.max;
}
#endif