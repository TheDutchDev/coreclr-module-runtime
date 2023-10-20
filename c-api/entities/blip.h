#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wempty-body"
#pragma clang diagnostic ignored "-Wswitch"
#endif

#include "cpp-sdk/SDK.h"
#include "../data/types.h"
#include "../utils/export.h"
#include "../utils/strings.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

EXPORT_SHARED uint32_t Blip_GetID(alt::IBlip* blip);

EXPORT_SHARED alt::IWorldObject* Blip_GetWorldObject(alt::IBlip* blip);

EXPORT_SHARED uint8_t Blip_IsVisible(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetVisible(alt::IBlip* blip, uint8_t toggle);


EXPORT_SHARED uint8_t Blip_IsGlobal(alt::IBlip* blip);
EXPORT_SHARED uint8_t Blip_GetBlipType(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetBlipType(alt::IBlip* blip, uint8_t blipType);
EXPORT_SHARED void Blip_GetScaleXY(alt::IBlip* blip, vector2_t &scale);
EXPORT_SHARED void Blip_SetScaleXY(alt::IBlip* blip, vector2_t scale);
EXPORT_SHARED uint32_t Blip_GetDisplay(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetDisplay(alt::IBlip* blip, uint32_t display);
EXPORT_SHARED uint32_t Blip_GetSprite(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetSprite(alt::IBlip* blip, uint32_t sprite);
EXPORT_SHARED uint32_t Blip_GetColor(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetColor(alt::IBlip* blip, uint32_t color);
EXPORT_SHARED void Blip_GetSecondaryColor(alt::IBlip* blip, rgba_t &color);
EXPORT_SHARED void Blip_SetSecondaryColor(alt::IBlip* blip, rgba_t color);
EXPORT_SHARED uint32_t Blip_GetAlpha(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetAlpha(alt::IBlip* blip, uint32_t alpha);
EXPORT_SHARED uint16_t Blip_GetFlashTimer(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetFlashTimer(alt::IBlip* blip, uint16_t timer);
EXPORT_SHARED uint16_t Blip_GetFlashInterval(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetFlashInterval(alt::IBlip* blip, uint16_t interval);
EXPORT_SHARED uint8_t Blip_GetAsFriendly(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetAsFriendly(alt::IBlip* blip, uint8_t friendly);
EXPORT_SHARED uint8_t Blip_GetRoute(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetRoute(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetBright(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetBright(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint16_t Blip_GetNumber(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetNumber(alt::IBlip* blip, uint16_t number);
EXPORT_SHARED uint8_t Blip_GetShowCone(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetShowCone(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetFlashes(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetFlashes(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetFlashesAlternate(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetFlashesAlternate(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetAsShortRange(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetAsShortRange(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint32_t Blip_GetPriority(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetPriority(alt::IBlip* blip, uint32_t priority);
EXPORT_SHARED float Blip_GetRotation(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetRotation(alt::IBlip* blip, float rotation);
EXPORT_SHARED const char* Blip_GetGxtName(alt::IBlip* blip, int32_t& size);
EXPORT_SHARED void Blip_SetGxtName(alt::IBlip* blip, const char* name);
EXPORT_SHARED const char* Blip_GetName(alt::IBlip* blip, int32_t& size);
EXPORT_SHARED void Blip_SetName(alt::IBlip* blip, const char* name);
EXPORT_SHARED void Blip_GetRouteColor(alt::IBlip* blip, rgba_t &color);
EXPORT_SHARED void Blip_SetRouteColor(alt::IBlip* blip, rgba_t color);
EXPORT_SHARED uint8_t Blip_GetPulse(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetPulse(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetAsMissionCreator(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetAsMissionCreator(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetTickVisible(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetTickVisible(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetHeadingIndicatorVisible(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetHeadingIndicatorVisible(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetOutlineIndicatorVisible(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetOutlineIndicatorVisible(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetFriendIndicatorVisible(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetFriendIndicatorVisible(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetCrewIndicatorVisible(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetCrewIndicatorVisible(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint32_t Blip_GetCategory(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetCategory(alt::IBlip* blip, uint32_t category);
EXPORT_SHARED uint8_t Blip_GetAsHighDetail(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetAsHighDetail(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED uint8_t Blip_GetShrinked(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetShrinked(alt::IBlip* blip, uint8_t state);
EXPORT_SHARED void Blip_Fade(alt::IBlip* blip, uint32_t opacity, uint32_t duration);

EXPORT_SHARED uint8_t Blip_IsAttached(alt::IBlip* blip);
EXPORT_SHARED void* Blip_AttachedTo(alt::IBlip* blip, alt::IBaseObject::Type &type);

EXPORT_CLIENT uint32_t Blip_GetGameID(alt::IBlip* blip);
EXPORT_CLIENT uint8_t Blip_IsStreamedIn(alt::IBlip* blip);

EXPORT_SERVER void Blip_SetGlobal(alt::IBlip* blip, uint8_t state);
EXPORT_SERVER void Blip_AddTargetPlayer(alt::IBlip* blip, alt::IPlayer* player);
EXPORT_SERVER void Blip_RemoveTargetPlayer(alt::IBlip* blip, alt::IPlayer* player);
EXPORT_SERVER alt::IPlayer** Blip_GetTargets(alt::IBlip* blip, uint64_t& size);

EXPORT_SHARED uint8_t Blip_IsHiddenOnLegend(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetHiddenOnLegend(alt::IBlip* blip, uint8_t state);

EXPORT_SHARED uint8_t Blip_IsMinimalOnEdge(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetMinimalOnEdge(alt::IBlip* blip, uint8_t state);

EXPORT_SHARED uint8_t Blip_IsUseHeightIndicatorOnEdge(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetUseHeightIndicatorOnEdge(alt::IBlip* blip, uint8_t state);

EXPORT_SHARED uint8_t Blip_IsShortHeightThreshold(alt::IBlip* blip);
EXPORT_SHARED void Blip_SetShortHeightThreshold(alt::IBlip* blip, uint8_t state);

