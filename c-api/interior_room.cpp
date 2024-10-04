#include "interior_room.h"

#include "cpp-sdk/script-objects/IInteriorRoom.h"
#include "utils/macros.h"
#include "utils/strings.h"

CAPI_START()

#ifdef ALT_CLIENT_API
const char* InteriorRoom_GetName(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, int32_t& size)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	return AllocateString(room->GetName(), size);
}

uint32_t InteriorRoom_GetNameHash(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	return room->GetNameHash();
}

int32_t InteriorRoom_GetFlag(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	return room->GetFlag();
}

uint32_t InteriorRoom_GetTimecycle(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	return room->GetTimecycle();
}

void InteriorRoom_GetExtents(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex,
	interior_extend_info_t& extents)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	auto ext = room->GetExtents();
	extents.min = ext.min;
	extents.max = ext.max;
}

void InteriorRoom_SetFlag(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, int32_t flag)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	room->SetFlag(flag);
}

void InteriorRoom_SetTimecycle(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, uint32_t timecycle)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	room->SetTimecycle(timecycle);
}

void InteriorRoom_SetEntitiesExtents(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex, position_t min,
	position_t max)
{
	auto room = GetInteriorRoom(interiorId, roomValue, isIndex);
	alt::IInterior::ExtentInfo extentInfo;
	extentInfo.min = alt::Position(min.x, min.y, min.z);
	extentInfo.max = alt::Position(max.x, max.y, max.z);
	room->SetExtents(extentInfo);
}

std::shared_ptr<alt::IInteriorRoom> GetInteriorRoom(uint32_t interiorId, uint32_t roomValue, uint8_t isIndex)
{
	auto interior = alt::ICore::Instance().GetInterior(interiorId);
	std::shared_ptr<alt::IInteriorRoom> room;
	if (isIndex == 1)
	{
		room = interior->GetRoomByIndex(roomValue);
	}
	else
	{
		room = interior->GetRoomByHash(roomValue);
	}

	return room;
}
#endif