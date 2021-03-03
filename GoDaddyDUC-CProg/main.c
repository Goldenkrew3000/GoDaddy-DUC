#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

#include "extractConfig.h"
#include "getExternalIP.h"
#include "getGoDaddyIP.h"
#include "sendGoDaddyIP.h"

int main()
{
    // Create Variables
    char sExternalIpUrl[] = "https://ifconfig.me/all.json";
    char sGoDaddyIpUrlPartA[] = "https://api.godaddy.com/v1/domains/";
    char sGoDaddyIpUrlPartB[] = "/records/A/";

    char sGoDaddyDomain[1024];
    char sGoDaddyAName[1024];
    char sGoDaddyTTL[1024];
    char sGoDaddyType[1024];
    char sGoDaddyApiKey[1024];
    char sGoDaddyApiSecret[1024];

    char* oGoDaddyDomain = NULL;
    char* oGoDaddyAName = NULL;
    char* oGoDaddyTTL = NULL;
    char* oGoDaddyType = NULL;
    char* oGoDaddyApiKey = NULL;
    char* oGoDaddyApiSecret = NULL;

    // Create Receive Variables
    char* sExternalIp = "";
    char* sGoDaddyIp = "";
    char* sEquals = "";
    char* sGoDaddySend = "";

    // Print title
    printf("GoDaddy Dynamic IP Updater\n");
    printf("Malexty 2021\n");

    // Get JSON from config.json
    FILE *f;
    f = fopen("config.json", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *fString = malloc(fsize + 1);
    fread(fString, 1, fsize, f);
    fclose(f);
    fString[fsize] = 0;
    printf("%s\n", fString);

    // Get GoDaddy Domain from JSON
    oGoDaddyDomain = extract_godaddy_domain(fString);
    strcpy(sGoDaddyDomain, oGoDaddyDomain);
    printf("%s\n", sGoDaddyDomain);

    // Get GoDaddy A Name from JSON
    oGoDaddyAName = extract_godaddy_a_name(fString);
    strcpy(sGoDaddyAName, oGoDaddyAName);
    printf("%s\n", sGoDaddyAName);

    // Get GoDaddy TTL from JSON
    oGoDaddyTTL = extract_godaddy_ttl(fString);
    strcpy(sGoDaddyTTL, oGoDaddyTTL);
    printf("%s\n", sGoDaddyTTL);

    // Get GoDaddy Type from JSON
    oGoDaddyType = extract_godaddy_type(fString);
    strcpy(sGoDaddyType, oGoDaddyType);
    printf("%s\n", sGoDaddyType);

    // Get GoDaddy API Key from JSON
    oGoDaddyApiKey = extract_api_key(fString);
    strcpy(sGoDaddyApiKey, oGoDaddyApiKey);
    printf("%s\n", sGoDaddyApiKey);

    // Get GoDaddy API Secret from JSON
    oGoDaddyApiSecret = extract_api_secret(fString);
    strcpy(sGoDaddyApiSecret, oGoDaddyApiSecret);
    printf("%s\n", sGoDaddyApiSecret);

    // Receive External IP
    printf("Receiving External IP\n");
    sExternalIp = getExternalIP(sExternalIpUrl);
    printf("External IP: %s\n", sExternalIp);

    // Receive GoDaddy IP
    printf("Receiving GoDaddy IP\n");
    sGoDaddyIp = getGoDaddyIP(sGoDaddyDomain, sGoDaddyAName, sGoDaddyTTL, sGoDaddyApiKey, sGoDaddyApiSecret, sGoDaddyIpUrlPartA, sGoDaddyIpUrlPartB);
    printf("GoDaddy IP: %s\n", sGoDaddyIp);

    // Compare IPs
    printf("Comparing IPs\n");
    int result;
    result = strcmp(sExternalIp, sGoDaddyIp);

    // Send GoDaddy the External IP
    printf("Checking if IPs match\n");
    if (result != 0)
    {
        printf("IPs do not match, changing IP\n");
        sGoDaddySend = sendGoDaddyIP(sGoDaddyDomain, sGoDaddyAName, sGoDaddyTTL, sGoDaddyType, sGoDaddyApiKey, sGoDaddyApiSecret, sGoDaddyIpUrlPartA, sGoDaddyIpUrlPartB, sExternalIp);
        printf("%s\n", sGoDaddySend);
    } else {
        printf("IPs match\n");
    }

    printf("Freeing ExternalIP\n");
    free(sExternalIp); // Free memory after return from getExternalIP
    printf("Freeing GoDaddy\n");
    free(fString);
    free(sGoDaddyIp);
    free(oGoDaddyDomain);
    free(oGoDaddyAName);
    free(oGoDaddyTTL);
    free(oGoDaddyType);
    free(oGoDaddyApiKey);
    free(oGoDaddyApiSecret);

    return 0;
}
