# GoDaddy DUC Client
# Version 1.2
# Updates A Name on GoDaddy Domain Automatically
# Python 3.8
# This script is to be automatically run by another program at a certain time interval
# Lexi Geraghty 2020
# Email me at: goldenkrew3000@naver.com if you have any issues/suggestions

import urllib3
import json

# Debug Setting
# False = Debug off - No console output
# True = Debug on - Lots of console output
debug = True

# List Preconfigured Settings
# False = List Preconfigured Settings off
# True = List Preconfigured Settings on
preconfig_debug = True

# Recheck IP After Setting it Setting
# False
# True
recheck_after = True

# GoDaddy Account and Domain Settings
GoDaddy_Domain = "" # Your GoDaddy Domain (e.g. malexty.com)
GoDaddy_A_Name = "" # GoDaddy Domain A Name
GoDaddy_API_Key = "" # Production API Key
GoDaddy_API_Secret = "" # Production API Secret

# GoDaddy DNS Settings
GoDaddy_DNS_Name = "" # GoDaddy Domain A Name
GoDaddy_DNS_TTL = 600 # GoDaddy Domain TTL

# GoDaddy URLs
GoDaddy_API_URL_Part1 = "https://api.godaddy.com/v1/domains/"
GoDaddy_API_URL_Part2 = "/records/A/"

# External IP Grabber URL
ExternalIPUrl = "https://ifconfig.me/all.json"

if debug == True:
    if preconfig_debug == True:
        print("Preconfigured Settings:")
        print("Debug: " + str(debug))
        print("List Preconfigured Settings: " + str(preconfig_debug))
        print("Recheck DNS After Setting: " + str(recheck_after))
        print("Preconfigured GoDaddy Settings:")
        print("GoDaddy Domain: " + GoDaddy_Domain)
        print("GoDaddy A Name: " + GoDaddy_A_Name)
        print("GoDaddy API Key: " + GoDaddy_API_Key)
        print("GoDaddy API Secret: " + GoDaddy_API_Secret)
        print("Preconfigured GoDaddy URL Parts for concatenation throughout the script:")
        print("GoDaddy API URL Part 1: " + GoDaddy_API_URL_Part1)
        print("GoDaddy API URL Part 2: " + GoDaddy_API_URL_Part2)
        print("Preconfigured IP Grabber:")
        print("External IP Grabber: " + ExternalIPUrl)

# Generate GoDaddy Authorization Headers
GoDaddy_Headers = {
    "Content-Type": "application/json",
    "Accept-Type": "application/json",
    "Authorization": "sso-key " + GoDaddy_API_Key + ":" + GoDaddy_API_Secret
}

if debug == True:
    print("Generated GoDaddy Authorization Headers:")
    print(GoDaddy_Headers)

# Urllib3 initialization
http = urllib3.PoolManager()

# Get External IP from ExternalIPUrl
if debug == True:
    print("Grabbing External IP from " + ExternalIPUrl)

ExternalIPRequest = http.request('GET', ExternalIPUrl)
ExternalIPResponse = ExternalIPRequest.data.decode('utf-8')
ExternalIPJSON = json.loads(ExternalIPResponse)
ExternalIP = ExternalIPJSON["ip_addr"]

if debug == True:
    print("External IP JSON:")
    print(ExternalIPJSON)
    print("External IP:")
    print(ExternalIP)

# Generate GoDaddy API Domain A Name URL
GoDaddy_API_AName_URL = GoDaddy_API_URL_Part1 + GoDaddy_Domain + GoDaddy_API_URL_Part2 + GoDaddy_A_Name

if debug == True:
    print("Generated GoDaddy API Domain A Name URL:")
    print(GoDaddy_API_AName_URL)

# Recieve GoDaddy DNS Info from API
GoDaddy_AName_Req = http.request('GET', GoDaddy_API_AName_URL, headers=GoDaddy_Headers)
GoDaddy_AName_Res = GoDaddy_AName_Req.data.decode('utf-8')
GoDaddy_AName_JSON = json.loads(GoDaddy_AName_Res)
GoDaddy_DNS_IP = GoDaddy_AName_JSON[0]["data"]

if debug == True:
    print("GoDaddy DNS JSON:")
    print(GoDaddy_AName_JSON)
    print("Extracted GoDaddy DNS IP:")
    print(GoDaddy_DNS_IP)

# Check if the current External IP and the GoDaddy DNS IP match
if GoDaddy_DNS_IP == ExternalIP:
    # GoDaddy DNS IP and External IP match
    if debug == True:
        print("IPs Match:")
        print("Current External IP: " + ExternalIP)
        print("GoDaddy DNS IP: " + GoDaddy_DNS_IP)
else:
    # GoDaddy DNS IP and External IP dont match
    if debug == True:
        print("IPs Dont Match:")
        print("Current External IP: " + ExternalIP)
        print("GoDaddy DNS IP: " + GoDaddy_DNS_IP)
    
    # Generate GoDaddy DNS Data to send
    GoDaddy_DNS_Data = [{
        "data": ExternalIP,
        "name": GoDaddy_A_Name,
        "ttl": GoDaddy_DNS_TTL,
        "type": "A"
    }]

    if debug == True:
        print("Generated GoDaddy DNS Send Data")
        print(GoDaddy_DNS_Data)
    
    # Send Updated GoDaddy DNS Data to GoDaddy API
    GoDaddy_DNS_Update_Req = http.request('PUT', GoDaddy_API_AName_URL, headers=GoDaddy_Headers, body=json.dumps(GoDaddy_DNS_Data))
    GoDaddy_DNS_Update_Res = GoDaddy_DNS_Update_Req.data.decode('utf-8')
    GoDaddy_DNS_Update_Res_Status = GoDaddy_DNS_Update_Req.status

    if debug == True:
        print("GoDaddy DNS Update Response Status Code")
        print(GoDaddy_DNS_Update_Res_Status)
        if GoDaddy_DNS_Update_Res_Status == 200:
            print("Status Code " + str(GoDaddy_DNS_Update_Res_Status) + " means it worked!")

    if recheck_after == True:
        # Recheck setting recheck_after is on, will check API again
        if debug == True:
            print("Recheck setting")
        
        # Receive GoDaddy DNS IP from API to check if it set
        GoDaddy_DNS_Recheck_Req = http.request('GET', GoDaddy_API_AName_URL, headers=GoDaddy_Headers)
        GoDaddy_DNS_Recheck_Res = GoDaddy_DNS_Recheck_Req.data.decode('utf-8')
        GoDaddy_DNS_Recheck_JSON = json.loads(GoDaddy_DNS_Recheck_Res)
        GoDaddy_DNS_Recheck_IP = GoDaddy_DNS_Recheck_JSON[0]["data"]

        if debug == True:
            print("GoDaddy DNS JSON:")
            print(GoDaddy_DNS_Recheck_JSON)
            print("GoDaddy DNS IP Received from API for Rechecking:")
            print(GoDaddy_DNS_Recheck_IP)
        
        # Check GoDaddy DNS IP against Current External IP
        if GoDaddy_DNS_Recheck_IP == ExternalIP:
            # GoDaddy DNS IP is the same as External IP
            print("New GoDaddy DNS IP and External IP Match:")
            print("New GoDaddy DNS IP: " + GoDaddy_DNS_Recheck_IP)
            print("External IP: " + ExternalIP)
        else:
            # GoDaddy DNS IP is not the same as External IP
            print("New GoDaddy DNS IP and External IP Dont Match:")
            print("New GoDaddy DNS IP: " + GoDaddy_DNS_Recheck_IP)
            print("External IP: " + ExternalIP)
            print("THE IP WILL MOST LIKELY SET, JUST NOT INSTANTLY")

if debug == True:
    print("Program finished!")
