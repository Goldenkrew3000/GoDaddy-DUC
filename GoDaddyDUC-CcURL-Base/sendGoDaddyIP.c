#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "cJSON.h"
#include "sendGoDaddyIP.h"

char* sendGoDaddyIP(char* sGoDaddyDomain, char* sGoDaddyAName, char* sGoDaddyTTL, char* sGoDaddyType, char* sGoDaddyApiKey, char* sGoDaddyApiSecret, char* sGoDaddyIpUrlPartA, char* sGoDaddyIpUrlPartB, char* sExternalIp)
{
    printf("Sending GoDaddy IP\n");

    
    printf("Received data:\n");
    printf("GoDaddy Domain: %s\n", sGoDaddyDomain);
    printf("GoDaddy A Name: %s\n", sGoDaddyAName);
    printf("GoDaddy TTL: %s\n", sGoDaddyTTL);
    printf("GoDaddy Type: %s\n", sGoDaddyType);
    printf("GoDaddy API Key: %s\n", sGoDaddyApiKey);
    printf("GoDaddy API Secret: %s\n", sGoDaddyApiSecret);
    printf("GoDaddy IP Url Part A: %s\n", sGoDaddyIpUrlPartA);
    printf("GoDaddy IP Url Part B: %s\n", sGoDaddyIpUrlPartB);
    printf("External IP: %s\n", sExternalIp);
    
    // Well the above checks if all the variables sent to this function are in the right order, which it appears they are

    CURL *curl;
    CURLcode res;
    struct curl_slist* headers = NULL;

    curl = curl_easy_init();

    // Generate Headers
    char cAuthGenHeader[1024];
    strcpy(cAuthGenHeader, "Authorization: sso-key ");
    strcat(cAuthGenHeader, sGoDaddyApiKey);
    strcat(cAuthGenHeader, ":");
    strcat(cAuthGenHeader, sGoDaddyApiSecret);
    char* sAuthGenHeader = &cAuthGenHeader[0];

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept-Type: application/json");
    headers = curl_slist_append(headers, sAuthGenHeader);

    // Generate JSON String to send
    int iGoDaddyTTL = atoi(sGoDaddyTTL);
    cJSON* root = NULL;
    cJSON* innerRoot = NULL;
    cJSON* jsonData = NULL;
    cJSON* jsonName = NULL;
    cJSON* jsonTTL = NULL;
    cJSON* jsonType = NULL;
    char* jsonString = NULL;

    jsonData = cJSON_CreateString(sExternalIp);
    jsonName = cJSON_CreateString(sGoDaddyAName);
    jsonTTL = cJSON_CreateNumber(iGoDaddyTTL);
    jsonType = cJSON_CreateString(sGoDaddyType);

    root = cJSON_CreateArray();
    innerRoot = cJSON_CreateObject();
    cJSON_AddItemToArray(root, innerRoot);
    cJSON_AddItemToObject(innerRoot, "data", jsonData);
    cJSON_AddItemToObject(innerRoot, "name", jsonName);
    cJSON_AddItemToObject(innerRoot, "ttl", jsonTTL);
    cJSON_AddItemToObject(innerRoot, "type", jsonType);

    jsonString = cJSON_Print(root);
    printf("%s\n", jsonString);

    // Generate URL
    char cURLGen[1024];
    strcpy(cURLGen, sGoDaddyIpUrlPartA);
    strcat(cURLGen, sGoDaddyDomain);
    strcat(cURLGen, sGoDaddyIpUrlPartB);
    strcat(cURLGen, sGoDaddyAName);
    char* sURLGen = &cURLGen[0];
    printf("%s\n", sURLGen);

    // Connect and send the data
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, sURLGen);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString);
    }

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }

    // Same reason in getExternalIP.c
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(jsonString); // This one is specific to this file, but still just clears memory leaks

    curl_global_cleanup();
    cJSON_Delete(root);

    return "Done";
}
