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

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
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

char* getExternalIP(char* sExternalIpUrl) {
    CURL *curl;
    CURLcode res;

    get_request req = {.buffer = NULL, .len = 0, .buflen = 0};

    curl = curl_easy_init(); // This causes a memory leak but there is nothing you can do about it yet

    struct curl_slist* contentheader = NULL;
    contentheader = curl_slist_append(contentheader, "Content-Type: application/json");
    contentheader = curl_slist_append(contentheader, "Accept-Type: application/json");

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, "https://ifconfig.me/all.json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, contentheader);
        req.buffer = malloc(CHUNK_SIZE);
        req.buflen = CHUNK_SIZE;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
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

    // Added the two below because it gets rid of the valgrind memory leak warning
    curl_slist_free_all(contentheader);
    curl_easy_cleanup(curl);

    curl_global_cleanup();

    cJSON *json = cJSON_Parse(req.buffer);
    char *string = cJSON_Print(json);
    //printf("%s\n", string);

    cJSON *ipaddr = cJSON_GetObjectItem(json, "ip_addr");
    char *sIpaddr = cJSON_Print(ipaddr);
    
    if (sIpaddr[0] == '"') 
    memmove(sIpaddr, sIpaddr+1, strlen(sIpaddr));

    sIpaddr[strlen(sIpaddr)-1] = '\0'; // Remove last character from string

    // The stuff below is to free memory to avoid memory leaks
    cJSON_Delete(json);
    free(req.buffer);
    free(string);

    return sIpaddr;
}
