/*
// extractConfig.c
// What this file does:
// > Receives JSON Text, Parses it, and Returns the result
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "extractConfig.h"

// Extract GoDaddy Domain from config json
char* extract_godaddy_domain(char* jsonString)
{
    cJSON *oJSON = cJSON_Parse(jsonString);
    cJSON *oGoDaddy = cJSON_GetObjectItem(oJSON, "godaddy");
    cJSON *oDomainSettings = cJSON_GetObjectItem(oGoDaddy, "domain-settings");
    cJSON *oDomainName = cJSON_GetObjectItem(oDomainSettings, "domain");
    char *sDomainName = cJSON_Print(oDomainName);
    if (sDomainName[0] == '"')
    {
        memmove(sDomainName, sDomainName + 1, strlen(sDomainName));
    }
    sDomainName[strlen(sDomainName) - 1] = '\0';
    cJSON_Delete(oJSON);
    return sDomainName;
}

// Extract GoDaddy A Name from config json
char* extract_godaddy_a_name(char* jsonString)
{
    cJSON *oJSON = cJSON_Parse(jsonString);
    cJSON *oGoDaddy = cJSON_GetObjectItem(oJSON, "godaddy");
    cJSON *oDomainSettings = cJSON_GetObjectItem(oGoDaddy, "domain-settings");
    cJSON *oAName = cJSON_GetObjectItem(oDomainSettings, "a-name");
    char *sAName = cJSON_Print(oAName);
    if (sAName[0] == '"')
    {
        memmove(sAName, sAName + 1, strlen(sAName));
    }
    sAName[strlen(sAName) - 1] = '\0';
    cJSON_Delete(oJSON);
    return sAName;
}

// Extract GoDaddy TTL from config json
char* extract_godaddy_ttl(char* jsonString)
{
    cJSON *oJSON = cJSON_Parse(jsonString);
    cJSON *oGoDaddy = cJSON_GetObjectItem(oJSON, "godaddy");
    cJSON *oDomainSettings = cJSON_GetObjectItem(oGoDaddy, "domain-settings");
    cJSON *oTTL = cJSON_GetObjectItem(oDomainSettings, "ttl");
    char *sTTL = cJSON_Print(oTTL);
    if (sTTL[0] = '"')
    {
        memmove(sTTL, sTTL + 1, strlen(sTTL));
    }
    sTTL[strlen(sTTL) - 1] = '\0';
    cJSON_Delete(oJSON);
    return sTTL;
}

// Extract GoDaddy Type from config json
char* extract_godaddy_type(char* jsonString)
{
    cJSON *oJSON = cJSON_Parse(jsonString);
    cJSON *oGoDaddy = cJSON_GetObjectItem(oJSON, "godaddy");
    cJSON *oDomainSettings = cJSON_GetObjectItem(oGoDaddy, "domain-settings");
    cJSON *oType = cJSON_GetObjectItem(oDomainSettings, "type");
    char *sType = cJSON_Print(oType);
    if (sType[0] = '"')
    {
        memmove(sType, sType + 1, strlen(sType));
    }
    sType[strlen(sType) - 1] = '\0';
    cJSON_Delete(oJSON);
    return sType;
}

// Extract GoDaddy API Key from config json
char* extract_api_key(char* jsonString)
{
    cJSON *oJSON = cJSON_Parse(jsonString);
    cJSON *oGoDaddy = cJSON_GetObjectItem(oJSON, "godaddy");
    cJSON *oApiSettings = cJSON_GetObjectItem(oGoDaddy, "api-settings");
    cJSON *oApiKey = cJSON_GetObjectItem(oApiSettings, "api-key");
    char *sApiKey = cJSON_Print(oApiKey);
    if (sApiKey[0] = '"')
    {
        memmove(sApiKey, sApiKey + 1, strlen(sApiKey));
    }
    sApiKey[strlen(sApiKey) - 1] = '\0';
    cJSON_Delete(oJSON);
    return sApiKey;
}

// Extract GoDaddy API Secret from config json
char* extract_api_secret(char* jsonString)
{
    cJSON *oJSON = cJSON_Parse(jsonString);
    cJSON *oGoDaddy = cJSON_GetObjectItem(oJSON, "godaddy");
    cJSON *oApiSettings = cJSON_GetObjectItem(oGoDaddy, "api-settings");
    cJSON *oApiSecret = cJSON_GetObjectItem(oApiSettings, "api-secret");
    char *sApiSecret = cJSON_Print(oApiSecret);
    if (sApiSecret[0] = '"')
    {
        memmove(sApiSecret, sApiSecret + 1, strlen(sApiSecret));
    }
    sApiSecret[strlen(sApiSecret) - 1] = '\0';
    cJSON_Delete(oJSON);
    return sApiSecret;
}
