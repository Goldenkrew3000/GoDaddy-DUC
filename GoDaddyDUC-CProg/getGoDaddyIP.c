#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define CHUNK_SIZE 2048

typedef struct {
    unsigned char *buffer;
    size_t len;
    size_t buflen;
} get_request;

size_t write_callback2(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    get_request *req = (get_request *) userdata;

    while (req->buflen < req->len + realsize + 1)
    {
        req->buffer = realloc(req->buffer, req->buflen + CHUNK_SIZE);
        req->buflen += CHUNK_SIZE;
    }

    memcpy(&req->buffer[req->len], ptr, realsize);
    req->len += realsize;
    req->buffer[req->len] = 0;
    return realsize;
}

char* getGoDaddyIP(char* sGoDaddyDomain, char* sGoDaddyAName, char* sGoDaddyTTL, char* sGoDaddyApiKey, char* sGoDaddyApiSecret, char* sGoDaddyIpUrlPartA, char* sGoDaddyIpUrlPartB) {
    CURL *curl;
    CURLcode res;

    get_request req = {.buffer = NULL, .len = 0, .buflen = 0};

    // Need to setup URL and headers
    char cGoDaddyUrl[1024];
    strcpy(cGoDaddyUrl, sGoDaddyIpUrlPartA);
    strcat(cGoDaddyUrl, sGoDaddyDomain);
    strcat(cGoDaddyUrl, sGoDaddyIpUrlPartB);
    strcat(cGoDaddyUrl, sGoDaddyAName);
    //printf("URL: %s\n", cGoDaddyUrl);

    char cGoDaddyAuthorization[1024];
    strcpy(cGoDaddyAuthorization, "Authorization: sso-key ");
    strcat(cGoDaddyAuthorization, sGoDaddyApiKey);
    strcat(cGoDaddyAuthorization, ":");
    strcat(cGoDaddyAuthorization, sGoDaddyApiSecret);
    //printf("Authorization: %s\n", cGoDaddyAuthorization);

    char* sGoDaddyUrl = &cGoDaddyUrl[0];
    char* sGoDaddyAuthorization = &cGoDaddyAuthorization[0];

    //printf("URL: %s\n", sGoDaddyUrl);
    //printf("Authorization: %s\n", sGoDaddyAuthorization);

    curl = curl_easy_init(); // This causes a memory leak but there is nothing you can do about it yet

    struct curl_slist* contentheader = NULL;
    contentheader = curl_slist_append(contentheader, "Content-Type: application/json");
    contentheader = curl_slist_append(contentheader, "Accept-Type: application/json");
    contentheader = curl_slist_append(contentheader, sGoDaddyAuthorization);

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_URL, sGoDaddyUrl);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, contentheader);
        req.buffer = malloc(CHUNK_SIZE);
        req.buflen = CHUNK_SIZE;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback2);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);
    }

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }

    //printf("Result: %u\n", res);
    //printf("Total bytes received: %zu\n", req.len);
    //printf("Received:\n%s\n", req.buffer);

    // Same reason in getExternalIP.c
    curl_slist_free_all(contentheader);
    curl_easy_cleanup(curl);

    curl_global_cleanup();

    cJSON *goDaddyJson = cJSON_Parse(req.buffer);
    char *sGoDaddyJson = cJSON_Print(goDaddyJson);
    //printf("%s\n", sGoDaddyJson);

    cJSON *goDaddyJsonArray = cJSON_GetArrayItem(goDaddyJson, 0);
    cJSON *goDaddyData = cJSON_GetObjectItem(goDaddyJsonArray, "data");
    char *sGoDaddyData = cJSON_Print(goDaddyData);
    
    if (sGoDaddyData[0] == '"') 
    memmove(sGoDaddyData, sGoDaddyData+1, strlen(sGoDaddyData));

    sGoDaddyData[strlen(sGoDaddyData)-1] = '\0'; // Remove last character from string

    //printf("%s\n", sGoDaddyData);

    // The stuff below is to free up memory to avoid memory leaks
    cJSON_Delete(goDaddyJson);
    free(sGoDaddyJson);
    free(req.buffer);

    return sGoDaddyData;
}
