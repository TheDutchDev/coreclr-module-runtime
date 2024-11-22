#pragma once


#include "cpp-sdk/IDiscordManager.h"

struct ClrDiscordUser {
    int64_t id = 0;
    char* username = nullptr;
    char* discriminator = nullptr;
    char* avatar = nullptr;

    ClrDiscordUser() = default;

    ClrDiscordUser(alt::IDiscordManager* discordManager) {
        id = discordManager->GetUserID();

        auto usernameStr = discordManager->GetUsername();
        username = new char[usernameStr.length() + 1];
        strcpy_s(username, usernameStr.length() + 1, usernameStr.c_str());

        auto discriminatorStr = discordManager->GetDiscriminator();
        discriminator = new char[discriminatorStr.length() + 1];
        strcpy_s(discriminator, discriminatorStr.length() + 1, discriminatorStr.c_str());

        auto avatarStr = discordManager->GetAvatar();
        avatar = new char[avatarStr.length() + 1];
        strcpy_s(avatar, avatarStr.length() + 1,avatarStr.c_str());
    }

    ~ClrDiscordUser() {
        delete[] username;
        delete[] discriminator;
        delete[] avatar;
    }
};