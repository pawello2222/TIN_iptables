//
// Created by mmoraczynski on 15.12.15.
//
#include "auth.h"
#include <functional>
#include <stdlib.h>
#include <algorithm>
#include "../Handler/Validator.h"

char* Authorization::strToMd5(std::string toHash)
{
    unsigned char digest[16];
    char *out = (char*)malloc(33);
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, toHash.c_str(), toHash.length());
    MD5_Final(digest, &md5);

    for (int i = 0; i < 16; ++i)
    {
        snprintf(&(out[i*2]), 16*3, "%02x", (unsigned int)digest[i]);
    }

    return out;
}


std::string Authorization::generateChallenge()
{
    srand( time( NULL ) );
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(this->m_challengeLength,0);
    std::generate_n( str.begin(), this->m_challengeLength , randchar );
    return str;
}


Json::Value Authorization::loginInit(std::string username)
{
    Json::Value response;
    Configuration &config = Configuration::getInstance();

    if (!config.getUserPassword(username).empty()) {
        std::string challenge = generateChallenge();
        response["error_code"] = responseCode::RESPONSE_CODE_OK;
        response["error_message"] = "OK";
        response["challenge"] = challenge;
    }
    else
    {
        response["error_code"] = responseCode::RESPONSE_CODE_INVALID_USERNAME;
        response["error_message"] = "Specified username not exist";
        response["challenge"] = "";
    }
    return response;
}


bool Authorization::authorize(std::string username,std::string hash, std::string challenge)
{
    Json::Value response;
    Configuration &config = Configuration::getInstance();
    std::string password = config.getUserPassword(username);
    std::string localHash = strToMd5(password + challenge);
    return localHash == hash;
}